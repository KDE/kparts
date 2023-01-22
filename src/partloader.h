/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPARTS_PARTLOADER_H
#define KPARTS_PARTLOADER_H

#include <KPluginFactory>
#include <KPluginMetaData>
#include <QObject>
#include <QVector>
#include <kparts/kparts_export.h>

namespace KParts
{
/**
 * Helper methods for locating and loading parts.
 * This is based upon KPluginFactory, but it takes
 * care of querying by mimetype, sorting the available parts by builtin
 * preference and by user preference.
 * @since 5.69
 */
namespace PartLoader
{
namespace Private
{

enum ErrorType {
    CouldNotLoadPlugin,
    NoPartFoundForMimeType,
    NoPartInstantiatedForMimeType,
};

/**
 * @internal
 * @param errorString translated, user-visible error string
 * @param errorText untranslated error text
 * @param argument argument for the text
 */
KPARTS_EXPORT void getErrorStrings(QString *errorString, QString *errorText, const QString &argument, ErrorType type);

#if KPARTS_ENABLE_DEPRECATED_SINCE(5, 88)
/**
 * Helper for PartLoader::createPartInstanceForMimeType<T>
 * @internal
 */
KPARTS_EXPORT QObject *createPartInstanceForMimeTypeHelper(const char *iface, const QString &mimeType, QWidget *parentWidget, QObject *parent, QString *error);
#endif
}

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
 * use \ref instantiatePart()
 *
 * @since 5.69
 */
KPARTS_EXPORT QVector<KPluginMetaData> partsForMimeType(const QString &mimeType);

/**
 * Attempts to create a KPart from the given metadata.
 *
 * @code
 * KPluginFactory::Result<MyPart> result = KParts::PartLoader::instantiatePart<MyPart>(metaData, parentWidget, parent, args);
 * if (result) {
 *     // result.plugin is valid and can be accessed
 * } else {
 *     // result contains information about the error
 * }
 * @endcode
 * If there is no extra error handling needed the plugin can be directly accessed and checked if it is a nullptr
 * @code
 * if (auto plugin = KParts::PartLoader::instantiatePart<MyPart>(metaData, parentWidget, parent, args).plugin) {
 *     // The plugin is valid and can be accessed
 * }
 * @endcode
 * @param data KPluginMetaData from which the plugin should be loaded
 * @param parentWidget The parent widget
 * @param parent The parent object
 * @param args A list of arguments to be passed to the part
 * @return Result object which contains the plugin instance and potentially error information
 * @since 5.100
 */
template<typename T>
static KPluginFactory::Result<T>
instantiatePart(const KPluginMetaData &data, QWidget *parentWidget = nullptr, QObject *parent = nullptr, const QVariantList &args = {})
{
    KPluginFactory::Result<T> result;
    KPluginFactory::Result<KPluginFactory> factoryResult = KPluginFactory::loadFactory(data);
    if (!factoryResult.plugin) {
        result.errorString = factoryResult.errorString;
        result.errorReason = factoryResult.errorReason;
        return result;
    }
    T *instance = factoryResult.plugin->create<T>(parentWidget, parent, args);
    if (!instance) {
        const QString fileName = data.fileName();
        Private::getErrorStrings(&result.errorString, &result.errorText, fileName, Private::CouldNotLoadPlugin);
        result.errorReason = KPluginFactory::INVALID_KPLUGINFACTORY_INSTANTIATION;
    } else {
        result.plugin = instance;
    }
    return result;
}

/**
 * Use this method to create a KParts part. It will try to create an object which inherits
 * @param T.
 *
 * @code
 * KPluginFactory::Result<KParts::ReadOnlyPart> result = KParts::PartLoader::instantiatePartForMimeType<KParts::ReadOnlyPart>(mimeType, parentWidget, parent,
 * args);
 * if (result) {
 *     // result.plugin is valid and can be accessed
 * } else {
 *     // result contains information about the error
 * }
 * @endcode
 * If there is no extra error handling needed the plugin can be directly accessed and checked if it is a nullptr
 * @code
 * if (auto plugin = KParts::PartLoader::instantiatePartForMimeType<KParts::ReadOnlyPart>(mimeType, parentWidget, parent, args).plugin) {
 *     // The plugin is valid and can be accessed
 * }
 *
 * @tparam T The interface for which an object should be created. The object will inherit @param T.
 * @param mimeType The mimetype for which we need a KParts.
 * @param parentWidget The parent widget for the part's widget.
 * @param parent The parent of the part.
 * @return Result object which contains the plugin instance and potentially error information
 * @since 5.100
 */
template<class T>
static KPluginFactory::Result<T>
instantiatePartForMimeType(const QString &mimeType, QWidget *parentWidget = nullptr, QObject *parent = nullptr, const QVariantList &args = {})
{
    const QVector<KPluginMetaData> plugins = KParts::PartLoader::partsForMimeType(mimeType);

    if (plugins.isEmpty()) {
        KPluginFactory::Result<T> errorResult;
        errorResult.errorReason = KPluginFactory::ResultErrorReason::INVALID_PLUGIN;
        Private::getErrorStrings(&errorResult.errorString, &errorResult.errorText, mimeType, Private::NoPartFoundForMimeType);

        return errorResult;
    }

    for (const KPluginMetaData &plugin : plugins) {
        const KPluginFactory::Result<T> result = instantiatePart<T>(plugin, parentWidget, parent, args);

        if (result) {
            return result;
        }
    }

    KPluginFactory::Result<T> errorResult;
    errorResult.errorReason = KPluginFactory::ResultErrorReason::INVALID_PLUGIN;
    Private::getErrorStrings(&errorResult.errorString, &errorResult.errorText, mimeType, Private::NoPartInstantiatedForMimeType);

    return errorResult;
}

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
    const KPluginFactory::Result<T> result = instantiatePartForMimeType<T>(mimeType, parentWidget, parent, {});

    if (result) {
        return result.plugin;
    }

    *error = result.errorString;

    return nullptr;
}

} // namespace
} // namespace

#endif
