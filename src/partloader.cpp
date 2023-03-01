/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "partloader.h"

#include "kparts_logging.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KService>
#include <KSharedConfig>

#include <QMimeDatabase>
#include <QMimeType>

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

QList<KPluginMetaData> KParts::PartLoader::partsForMimeType(const QString &mimeType)
{
    auto supportsMime = [&](const KPluginMetaData &md) {
        return md.supportsMimeType(mimeType);
    };
    QList<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("kf6/parts"), supportsMime);

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
        const auto getInitialPreference = [](const KPluginMetaData &data) {
            return data.rawData().value(QLatin1String("KPlugin")).toObject().value(QLatin1String("InitialPreference")).toInt();
        };
        return getInitialPreference(left) > getInitialPreference(right);
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
