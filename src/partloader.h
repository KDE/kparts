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

#ifndef KPARTS_PARTLOADER_H
#define KPARTS_PARTLOADER_H

#include <kparts/kparts_export.h>
#include <QVector>
#include <QObject>
#include <KPluginMetaData>

namespace KParts
{

/**
 * Helper methods for locating and loading parts.
 * This is based upon KPluginLoader and KPluginFactory, but it takes
 * care of querying by mimetype, sorting the available parts by builtin
 * preference and by user preference.
 * @since 5.69
 */
namespace PartLoader
{
    namespace Private
    {
        /**
         * Helper for PartLoader::createPartInstanceForMimeType<T>
         * @internal
         */
        KPARTS_EXPORT QObject *createPartInstanceForMimeTypeHelper(const char *iface, const QString &mimeType, QWidget *parentWidget, QObject *parent, QString *error);
    }

    /**
     * Locate all available KParts for a mimetype.
     * @return a list of plugin metadata, sorted by preference.
     * This takes care both of the builtin preference (set by developers)
     * and of user preference (stored in mimeapps.list).
     *
     * This uses KPluginLoader::findPlugins, i.e. it requires the parts to
     * provide the metadata as JSON embedded into the plugin.
     * Until KF6, however, it also supports .desktop files as a fallback solution.
     *
     * To load a part from one of the KPluginMetaData instances returned here,
     * simply do
     * @code
     *     KPluginLoader loader(metaData.fileName());
     *     m_part = loader.factory()->create<KParts::ReadOnlyPart>(parentWidget, parent);
     * @endcode
     *
     * @since 5.69
     */
    KPARTS_EXPORT QVector<KPluginMetaData> partsForMimeType(const QString &mimeType);

    /**
     * Use this method to create a KParts part. It will try to create an object which inherits
     * \p T.
     *
     * Example:
     * \code
     *     QString errorString;
     *     m_part = KParts::PartLoader::createPartInstanceForMimeType<KParts::ReadOnlyPart>(
     *                   mimeType, this, this, &errorString);
     *     if (m_part) {
     *         layout->addWidget(m_part->widget()); // Integrate the widget
     *         createGUI(m_part); // Integrate the actions
     *         m_part->openUrl(url);
     *     } else {
     *         qWarning() << errorString;
     *     }
     * \endcode
     *
     * \tparam T The interface for which an object should be created. The object will inherit \p T.
     * \param mimeType The mimetype for which we need a KParts.
     * \param parentWidget The parent widget for the part's widget.
     * \param parent The parent of the part.
     * \param error Optional output parameter, it will be set to the error string, if any.
     * \returns A pointer to the created object is returned, or @c nullptr if an error occurred.
     * @since 5.69
     */
    template <class T>
    static T *createPartInstanceForMimeType(const QString &mimeType, QWidget *parentWidget = nullptr, QObject *parent = nullptr,
                                            QString *error = nullptr)
    {
        QObject *o = Private::createPartInstanceForMimeTypeHelper(T::staticMetaObject.className(), mimeType, parentWidget, parent, error);
        T *part = qobject_cast<T *>(o);
        if (!part) {
            delete o;
        }
        return part;
    }

} // namespace
} // namespace

#endif
