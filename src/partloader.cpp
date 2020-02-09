/* This file is part of the KDE project
   Copyright (C) 2020 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "partloader.h"

#include <KConfigGroup>
#include <KService>
#include <KSharedConfig>
#include <KPluginLoader>
#include <KPluginFactory>
#include <KLocalizedString>
#include <QDebug>
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
        qDebug() << "mimeapps.list specifies unknown service" << desktopFile;
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
        if (mime == parent)
            return 0;
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
    auto supportsMime = [&](const KPluginMetaData &md) { return md.supportsMimeType(mimeType); };
    QVector<KPluginMetaData> plugins = KPluginLoader::findPlugins(QStringLiteral("kf5/parts"), supportsMime);

#if KPARTS_VERSION <= QT_VERSION_CHECK(5, 900, 0)
    // KF5 compat code

    // I would compare library filenames, but KPluginMetaData::fileName looks like kf5/kparts/okteta and KService::library() is a full path
    // The user actually sees translated names, let's ensure those don't look duplicated in the list.
    auto isPluginForName = [](const QString &name) {
        return [name](const KPluginMetaData &plugin) { return plugin.name() == name; };
    };

    const KService::List offers = KMimeTypeTrader::self()->query(mimeType, QStringLiteral("KParts/ReadOnlyPart"));
    for (const KService::Ptr &service : offers) {
        KPluginInfo info(service);
        if (info.isValid()) {
            if (std::find_if(plugins.cbegin(), plugins.cend(), isPluginForName(info.name())) == plugins.cend()) {
                plugins.append(info.toMetaData());
            }
        }
    }
#endif

    auto orderPredicate = [&](const KPluginMetaData &left, const KPluginMetaData &right) {
        // We filtered based on "supports mimetype", but this didn't order from most-specific to least-specific.
        const int leftDistance = pluginDistanceToMimeType(left, mimeType);
        const int rightDistance = pluginDistanceToMimeType(right, mimeType);
        if (leftDistance < rightDistance)
            return true;
        if (leftDistance > rightDistance)
            return false;
        // Plugins who support the same mimetype are then sorted by initial preference
        return left.initialPreference() > right.initialPreference();
    };
    std::sort(plugins.begin(), plugins.end(), orderPredicate);

    const QStringList userParts = partsFromUserPreference(mimeType);
    if (!userParts.isEmpty()) {
        //for (const KPluginMetaData &plugin : plugins) {
        //    qDebug() << "unsorted:" << plugin.fileName() << plugin.initialPreference();
        //}
        const auto defaultPlugins = plugins;
        plugins.clear();
        for (const QString &userPart : userParts) { // e.g. kf5/parts/gvpart
            auto matchesLibrary = [&](const KPluginMetaData &plugin) { return plugin.fileName().contains(userPart); };
            auto it = std::find_if(defaultPlugins.begin(), defaultPlugins.end(), matchesLibrary);
            if (it != defaultPlugins.end()) {
                plugins.push_back(*it);
            } else {
                qDebug() << "Part not found" << userPart;
            }
        }
        // In case mimeapps.list lists "nothing good", append the default set to the end as fallback
        plugins += defaultPlugins;
    }

    //for (const KPluginMetaData &plugin : plugins) {
    //    qDebug() << plugin.fileName() << plugin.initialPreference();
    //}
    return plugins;
}

// KF6 TODO: make create(const char* iface...) public in KPluginFactory, remove this hack
class KPluginFactoryHack : public KPluginFactory
{
public:
     QObject *create(const char *iface, QWidget *parentWidget, QObject *parent, const QVariantList &args, const QString &keyword) override {
         return KPluginFactory::create(iface, parentWidget, parent, args, keyword);
     }
};

QObject *KParts::PartLoader::Private::createPartInstanceForMimeTypeHelper(const char *iface, const QString &mimeType, QWidget *parentWidget, QObject *parent, QString *error)
{
    const QVector<KPluginMetaData> plugins = KParts::PartLoader::partsForMimeType(mimeType);
    for (const KPluginMetaData &plugin : plugins) {
        KPluginLoader pluginLoader(plugin.fileName());
        // ## How can the keyword feature work with JSON metadata?
        // ## Several desktop files could point to the same .so file, but a .so file only has one metadata builtin.
        // ## Unlikely to be a problem here (multiple KParts in the same .so file?), but to be solved for KCMs with an array of "KPlugin" in the JSON file...
        // ## It would then be used by KPluginLoader::findPlugins, and we'd use plugin.keyword() here.
        const QString pluginKeyword;
        KPluginFactory *factory = pluginLoader.factory();
        if (factory) {
            QObject *obj = static_cast<KPluginFactoryHack *>(factory)->create(iface, parentWidget, parent, QVariantList(), pluginKeyword);
            if (error) {
                if (!obj) {
                    *error = i18n("The plugin '%1' does not provide an interface '%2' with keyword '%3'",
                                  plugin.fileName(), QString::fromLatin1(iface), pluginKeyword);
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
}
