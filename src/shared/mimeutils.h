/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QMimeDatabase>

#include <KPluginMetaData>

// A plugin can support N mimetypes. Pick the one that is closest to @parent in the inheritance tree
// and return how far it is from that parent (0 = same mimetype, 1 = direct child, etc.)
static inline int pluginDistanceToMimeType(const KPluginMetaData &md, const QString &parent)
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

static inline auto orderPredicateForMimetype(const QString& mimeType)
{
    return [mimeType = mimeType](const KPluginMetaData &left, const KPluginMetaData &right) {
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
};
