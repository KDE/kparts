/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "partloader.h"

#include "kparts_logging.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KPluginLoader>
#include <KService>
#include <KSharedConfig>
#include <stack>

#include <kparts_version.h> // TODO KF6 REMOVE
#if KPARTS_VERSION <= QT_VERSION_CHECK(5, 900, 0)
#include <KMimeTypeTrader>
#include <KPluginInfo>
#include <QMimeDatabase>
#include <QMimeType>
#endif

// We still use desktop files for translated descriptions in keditfiletype,
// and desktop file names then end up in mimeapps.list.
// Alternatively, that KCM could be ported to read the descriptions from the JSON metadata?
// KF6 TODO: at least make the KCM write out library names (into a different config file)
// so we don't need to do the lookup here every time.
static QString pluginForDesktopFile(const QString &desktopFile)
{
    KService::Ptr service = KService::serviceByStorageId(desktopFile);
    if (!service) {
        qCDebug(KPARTSLOG) << "mimeapps.list specifies unknown service" << desktopFile;
        return {};
    }
    return service->library();
}

static QStringList partsFromUserPreference(const QString &mimeType)
{
    auto config = KSharedConfig::openConfig(QStringLiteral("mimeapps.list"));
    const QStringList desktopFiles = config->group(QStringLiteral("Added KDE Service Associations")).readXdgListEntry(mimeType);
    QStringList parts;
    parts.reserve(desktopFiles.count());
    for (const QString &desktopFile : desktopFiles) {
        const QString part = pluginForDesktopFile(desktopFile);
        if (!part.isEmpty()) {
            parts.append(part);
        }
    }
    return parts;
}

// A plugin can support N mimetypes. Pick the one that is closest to @parent in the inheritance tree
// and return how far it is from that parent (0 = same mimetype, 1 = direct child, etc.)
static int pluginDistanceToMimeType(const KPluginMetaData &md, const QString &parent)
{
    QMimeDatabase db;
    auto distanceToMimeType = [&](const QString &mime) {
        if (mime == parent) {
            return 0;
        }
        const QStringList ancestors = db.mimeTypeForName(mime).allAncestors();
        const int dist = ancestors.indexOf(parent);
        return dist == -1 ? 50 : dist + 1;
    };
    const QStringList mimes = md.mimeTypes();
    int minDistance = 50;
    for (const QString &mime : mimes) {
        minDistance = std::min(minDistance, distanceToMimeType(mime));
    }
    return minDistance;
}

QVector<KPluginMetaData> KParts::PartLoader::partsForMimeType(const QString &mimeType)
{
    auto supportsMime = [&](const KPluginMetaData &md) {
        return md.supportsMimeType(mimeType);
    };
    QVector<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("kf" QT_STRINGIFY(QT_VERSION_MAJOR) "/parts"), supportsMime);

#if KSERVICE_BUILD_DEPRECATED_SINCE(5, 0)
    // KF5 compat code

    // I would compare library filenames, but KPluginMetaData::fileName looks like kf5/kparts/okteta and KService::library() is a full path
    // The user actually sees translated names, let's ensure those don't look duplicated in the list.
    auto isPluginForName = [](const QString &name) {
        return [name](const KPluginMetaData &plugin) {
            return plugin.name() == name;
        };
    };

    QT_WARNING_PUSH
    QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
    QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
    const KService::List offers = KMimeTypeTrader::self()->query(mimeType, QStringLiteral("KParts/ReadOnlyPart"));
    for (const KService::Ptr &service : offers) {
        KPluginInfo info(service);
        if (info.isValid()) {
            if (std::find_if(plugins.cbegin(), plugins.cend(), isPluginForName(info.name())) == plugins.cend()) {
                plugins.append(info.toMetaData());
            }
        }
    }
    QT_WARNING_POP
#endif

    auto orderPredicate = [&](const KPluginMetaData &left, const KPluginMetaData &right) {
        // We filtered based on "supports mimetype", but this didn't order from most-specific to least-specific.
        const int leftDistance = pluginDistanceToMimeType(left, mimeType);
        const int rightDistance = pluginDistanceToMimeType(right, mimeType);
        if (leftDistance < rightDistance) {
            return true;
        }
        if (leftDistance > rightDistance) {
            return false;
        }
        // Plugins who support the same mimetype are then sorted by initial preference
        return left.initialPreference() > right.initialPreference();
    };
    std::sort(plugins.begin(), plugins.end(), orderPredicate);

    const QStringList userParts = partsFromUserPreference(mimeType);
    if (!userParts.isEmpty()) {
        // for (const KPluginMetaData &plugin : plugins) {
        //    qDebug() << "unsorted:" << plugin.fileName() << plugin.initialPreference();
        //}
        const auto defaultPlugins = plugins;
        plugins.clear();
        for (const QString &userPart : userParts) { // e.g. kf5/parts/gvpart
            auto matchesLibrary = [&](const KPluginMetaData &plugin) {
                return plugin.fileName().contains(userPart);
            };
            auto it = std::find_if(defaultPlugins.begin(), defaultPlugins.end(), matchesLibrary);
            if (it != defaultPlugins.end()) {
                plugins.push_back(*it);
            } else {
                qCDebug(KPARTSLOG) << "Part not found" << userPart;
            }
        }
        // In case mimeapps.list lists "nothing good", append the default set to the end as fallback
        plugins += defaultPlugins;
    }

    // for (const KPluginMetaData &plugin : plugins) {
    //    qDebug() << plugin.fileName() << plugin.initialPreference();
    //}
    return plugins;
}

void KParts::PartLoader::Private::getErrorStrings(QString *errorString, QString *errorText, const QString &argument, ErrorType type)
{
    switch (type) {
    case CouldNotLoadPlugin:
        *errorString = i18n("KPluginFactory could not load the plugin: %1", argument);
        *errorText = QStringLiteral("KPluginFactory could not load the plugin: %1").arg(argument);
        break;
    case NoPartFoundForMimeType:
        *errorString = i18n("No part was found for mimeType %1", argument);
        *errorText = QStringLiteral("No part was found for mimeType %1").arg(argument);
        break;
    case NoPartInstantiatedForMimeType:
        *errorString = i18n("No part could be instantiated for mimeType %1", argument);
        *errorText = QStringLiteral("No part could be instantiated for mimeType %1").arg(argument);
        break;
    default:
        qCWarning(KPARTSLOG) << "PartLoader::Private::getErrorStrings got unexpected error type" << type;
        break;
    }
};

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 88)
class KPluginFactoryHack : public KPluginFactory
{
public:
    QObject *create(const char *iface, QWidget *parentWidget, QObject *parent, const QVariantList &args, const QString &keyword) override
    {
        return KPluginFactory::create(iface, parentWidget, parent, args, keyword);
    }
};

QObject *KParts::PartLoader::Private::createPartInstanceForMimeTypeHelper(const char *iface,
                                                                          const QString &mimeType,
                                                                          QWidget *parentWidget,
                                                                          QObject *parent,
                                                                          QString *error)
{
    QT_WARNING_PUSH
    QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
    QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
    const QVector<KPluginMetaData> plugins = KParts::PartLoader::partsForMimeType(mimeType);
    for (const KPluginMetaData &plugin : plugins) {
        KPluginLoader pluginLoader(plugin.fileName());
        const QString pluginKeyword;
        KPluginFactory *factory = pluginLoader.factory();
        if (factory) {
            QObject *obj = static_cast<KPluginFactoryHack *>(factory)->create(iface, parentWidget, parent, QVariantList(), pluginKeyword);
            if (error) {
                if (!obj) {
                    *error = i18n("The plugin '%1' does not provide an interface '%2' with keyword '%3'",
                                  plugin.fileName(),
                                  QString::fromLatin1(iface),
                                  pluginKeyword);
                } else {
                    error->clear();
                }
            }
            if (obj) {
                return obj;
            }
        } else if (error) {
            *error = pluginLoader.errorString();
        }
        pluginLoader.unload();
    }
    if (error) {
        *error = i18n("No part was found for mimeType %1", mimeType);
    }
    return nullptr;
    QT_WARNING_POP
}
#endif
