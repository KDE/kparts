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

static QList<KPluginMetaData> partsFromUserPreference(const QString &mimeType)
{
    auto config = KSharedConfig::openConfig(QStringLiteral("mimeapps.list"));
    const QStringList pluginIds = config->group(QStringLiteral("Added KDE Part Associations")).readXdgListEntry(mimeType);
    QList<KPluginMetaData> plugins;
    plugins.reserve(pluginIds.size());
    for (const QString &pluginId : pluginIds) {
        if (KPluginMetaData data(QLatin1String("kf6/parts/") + pluginId); data.isValid()) {
            plugins << data;
        }
    }
    return plugins;
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

    const QList<KPluginMetaData> userParts = partsFromUserPreference(mimeType);
    if (!userParts.isEmpty()) {
        plugins = userParts;
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
