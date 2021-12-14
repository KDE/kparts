/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPARTS_PARTLOADER_H
#define KPARTS_PARTLOADER_H

#include <KLocalizedString>
#include <KPluginFactory>
#include <KPluginMetaData>
#include <QObject>
#include <QVector>
#include <kparts/kparts_export.h>

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
#if KPARTS_ENABLE_DEPRECATED_SINCE(5, 88)
namespace Private
{
/**
 * Helper for PartLoader::createPartInstanceForMimeType<T>
 * @internal
 */
KPARTS_EXPORT QObject *createPartInstanceForMimeTypeHelper(const char *iface, const QString &mimeType, QWidget *parentWidget, QObject *parent, QString *error);
}
#endif

/**
 * Locate all available KParts for a mimetype.
 * @return a list of plugin metadata, sorted by preference.
 * This takes care both of the builtin preference (set by developers)
 * and of user preference (stored in mimeapps.list).
 *
 * This uses KPluginMetaData::findPlugins, i.e. it requires the parts to
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
template<class T>
static T *createPartInstanceForMimeType(const QString &mimeType, QWidget *parentWidget = nullptr, QObject *parent = nullptr, QString *error = nullptr)
{
    const QVector<KPluginMetaData> plugins = KParts::PartLoader::partsForMimeType(mimeType);
    for (const auto &plugin : plugins) {
        auto factory = KPluginFactory::loadFactory(plugin);
        if (factory) {
            if (T *part = factory.plugin->create<T>(parentWidget, parent, QVariantList())) {
                return part;
            } else if (error) {
                *error = i18n("The plugin '%1' does not provide an interface '%2'", //
                              plugin.fileName(),
                              QLatin1String(T::staticMetaObject.className()));
            }
        } else if (error) {
            *error = factory.errorString;
        }
    }
    if (error && error->isEmpty()) {
        *error = i18n("No part was found for mimeType %1", mimeType);
    }
    return nullptr;
}

} // namespace
} // namespace

#endif
