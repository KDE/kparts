/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef _KPARTS_PARTBASE_H
#define _KPARTS_PARTBASE_H

#include <kparts/kparts_export.h>

#include <QObject>

#include <KXMLGUIClient>
#include <memory>

// Internal:
// As KParts::PartBase is inherited by KParts::Part which also inheriting from QObject,
// which already has a protected d_ptr member, the macro Q_DECLARE_PRIVATE cannot be used
// as it references d_ptr without any class qualifier, which is ambiguous then.
#define KPARTS_DECLARE_PRIVATE(Class) Q_DECLARE_PRIVATE_D(PartBase::d_ptr, Class)

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
class KAboutData;
#endif

namespace KParts
{
class PartBasePrivate;

/**
 * @class PartBase partbase.h <KParts/PartBase>
 *
 * @short Base class for all parts.
 */
class KPARTS_EXPORT PartBase : virtual public KXMLGUIClient
{
    KPARTS_DECLARE_PRIVATE(PartBase)

public:
    /**
     *  Constructor.
     */
    PartBase();

    /**
     *  Destructor.
     */
    ~PartBase() override;

    /**
     *  Internal method. Called by KParts::Part to specify the parent object for plugin objects.
     *
     * @internal
     */
    void setPartObject(QObject *object);
    QObject *partObject() const;

#if KPARTS_ENABLE_DEPRECATED_SINCE(5, 77)
    /**
     * @deprecated Since 5.77, use Part::metaData() or KXMLGUIClient::componentName() instead.
     */
    KPARTS_DEPRECATED_VERSION(5, 77, "Use Part::metaData() or KXMLGUIClient::componentName() instead")
    KAboutData componentData() const;
#endif

protected:
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
    /**
     * Set the componentData(KAboutData) for this part.
     *
     * Call this *first* in the inherited class constructor.
     * @deprecated Since 5.77, use Part::setMetaData(const KPluginMetaData&)
     *             or KXMLGUIClient::setComponentName(const QString &, const QString &) instead.
     */
    KPARTS_DEPRECATED_VERSION(5,
                              77,
                              "Use Part::setMetaData(const KPluginMetaData&) or KXMLGUIClient::setComponentName(const QString &, const QString &) instead")
    virtual void setComponentData(const KAboutData &componentData);
#endif

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
    /**
     * Set the componentData(KAboutData) for this part.
     *
     * Call this *first* in the inherited class constructor.
     *
     * It is recommended to call setComponentData with loadPlugins set to false,
     * and to load plugins at the end of your part constructor (in the case of
     * KParts::MainWindow, plugins are automatically loaded in createGUI anyway,
     * so set loadPlugins to false for KParts::MainWindow as well).
     * @deprecated Since 5.77, use Part::setMetaData(const KPluginMetaData&)
     *             or KXMLGUIClient::setComponentName(const QString &, const QString &) instead.
     */
    KPARTS_DEPRECATED_VERSION(5,
                              77,
                              "Use Part::setMetaData(const KPluginMetaData&) or KXMLGUIClient::setComponentName(const QString &, const QString &) instead")
    virtual void setComponentData(const KAboutData &pluginData, bool loadPlugins);
#endif

#if KPARTS_ENABLE_DEPRECATED_SINCE(5, 77)
    /**
     * Load the Plugins honoring the PluginLoadingMode.
     *
     * If you call this method in an already constructed GUI (like when the user
     * has changed which plugins are enabled) you need to add the new plugins to
     * the KXMLGUIFactory:
     * \code
     * if( factory() )
     * {
     *   const QList<KParts::Plugin *> plugins = KParts::Plugin::pluginObjects(this);
     *   for (auto *plugin : plugins) {
     *      factory()->addClient(plugin);
     *   }
     * }
     * \endcode
     * @deprecated Since 5.77, use loadPlugins(QObject *parent, KXMLGUIClient *, const QString &) instead.
     */
    KPARTS_DEPRECATED_VERSION(5, 77, "Use loadPlugins(QObject *parent, KXMLGUIClient *, const QString &) instead")
    void loadPlugins(QObject *parent, KXMLGUIClient *parentGUIClient, const KAboutData &aboutData);
#endif

#if KPARTS_ENABLE_DEPRECATED_SINCE(5, 90)
    /**
     * Load the Plugins honoring the PluginLoadingMode.
     *
     * If you call this method in an already constructed GUI (like when the user
     * has changed which plugins are enabled) you need to add the new plugins to
     * the KXMLGUIFactory:
     * \code
     * if( factory() )
     * {
     *   const QList<KParts::Plugin *> plugins = KParts::Plugin::pluginObjects(this);
     *   for (auto *plugin : plugins) {
     *      factory()->addClient(plugin);
     *   }
     * }
     * \endcode
     *
     * @since 5.77
     * @deprecated Since 5.90, the concept of KPart plugins is deprecated, see docs of @ref KParts::Plugin class
     */
    KPARTS_DEPRECATED_VERSION(5, 90, "The concept of KPart plugins is deprecated, see docs of KParts::Plugin class")
    void loadPlugins(QObject *parent, KXMLGUIClient *parentGUIClient, const QString &parentInstanceName);
#endif

#if KPARTS_ENABLE_DEPRECATED_SINCE(5, 90)
    /**
     * We have three different policies, whether to load new plugins or not. The
     * value in the KConfig object of the KAboutData object always overrides
     * LoadPlugins and LoadPluginsIfEnabled.
     */
    enum PluginLoadingMode {
        /**
         * Don't load any plugins at all.
         */
        DoNotLoadPlugins = 0,
        /**
         * Load new plugins automatically. Can be
         * overridden by the plugin if it sets
         * EnabledByDefault=false in the corresponding
         * .desktop file.
         */
        LoadPlugins = 1,
        /**
         * New plugins are disabled by default. Can be
         * overridden by the plugin if it sets
         * EnabledByDefault=true in the corresponding
         * .desktop file.
         */
        LoadPluginsIfEnabled = 2,
    };

    /**
     * Set how plugins should be loaded
     * @param loadingMode see PluginLoadingMode
     *
     * For a KParts::Part: call this before setComponentData.
     * For a KParts::MainWindow: call this before createGUI.
     * @deprecated Since 5.90, the concept of KPart plugins is deprecated, see docs of @ref KParts::Plugin class
     */
    KPARTS_DEPRECATED_VERSION(5, 90, "The concept of KPart plugins is deprecated, see docs of KParts::Plugin class")
    void setPluginLoadingMode(PluginLoadingMode loadingMode);
#endif

#if KPARTS_ENABLE_DEPRECATED_SINCE(5, 103)
    /**
     * If you change the binary interface offered by your part, you can avoid crashes
     * from old plugins lying around by setting X-KDE-InterfaceVersion=2 in the
     * .desktop files of the plugins, and calling setPluginInterfaceVersion( 2 ), so that
     * the old plugins are not loaded. Increase both numbers every time a
     * binary incompatible change in the application's plugin interface is made.
     *
     * @param version the interface version that plugins must have in order to be loaded.
     *
     * For a KParts::Part: call this before setComponentData.
     * For a KParts::MainWindow: call this before createGUI.
     * @deprecated Since 5.90, the concept of KPart plugins is deprecated, see docs of @ref KParts::Plugin class
     */
    KPARTS_DEPRECATED_VERSION_BELATED(5, 103, 5, 90, "The concept of KPart plugins is deprecated, see docs of KParts::Plugin class")
    void setPluginInterfaceVersion(int version);
#endif

protected:
    PartBase(PartBasePrivate &dd);

    std::unique_ptr<PartBasePrivate> const d_ptr;

private:
    Q_DISABLE_COPY(PartBase)
};

} // namespace

#endif
