/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
             (C) 1999 David Faure <faure@kde.org>

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

#ifndef _KPARTS_PARTBASE_H
#define _KPARTS_PARTBASE_H

#include <kparts/kparts_export.h>

#include <QObject>

#include <kxmlguiclient.h>


#define KPARTS_DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(PartBase::d_ptr); } \
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(PartBase::d_ptr); } \
    friend class Class##Private;

class KAboutData;

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
    virtual ~PartBase();

    /**
     *  Internal method. Called by KParts::Part to specify the parent object for plugin objects.
     *
     * @internal
     */
    void setPartObject(QObject *object);
    QObject *partObject() const;

    KAboutData componentData() const;

protected:
    /**
     * Set the componentData(KAboutData) for this part.
     *
     * Call this *first* in the inherited class constructor,
     * because it loads the i18n catalogs.
     */
    virtual void setComponentData(const KAboutData &componentData);

    /**
     * Set the componentData(KAboutData) for this part.
     *
     * Call this *first* in the inherited class constructor,
     * because it loads the i18n catalogs.
     *
     * It is recommended to call setComponentData with loadPlugins set to false,
     * and to load plugins at the end of your part constructor (in the case of
     * KParts::MainWindow, plugins are automatically loaded in createGUI anyway,
     * so set loadPlugins to false for KParts::MainWindow as well).
     */
    virtual void setComponentData(const KAboutData &pluginData, bool loadPlugins);
    // TODO KDE5: merge the above two methods, using loadPlugins=true. Or better, remove loadPlugins
    // altogether and change plugins to call loadPlugins() manually at the end of their ctor.
    // In the case of KParts MainWindows, plugins are automatically loaded in createGUI anyway,
    // so setComponentData() should really not load the plugins.

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
        LoadPluginsIfEnabled = 2
    };

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
     */
    void loadPlugins(QObject *parent, KXMLGUIClient *parentGUIClient, const KAboutData &aboutData);

    /**
     * Set how plugins should be loaded
     * @param loadingMode see PluginLoadingMode
     *
     * For a KParts::Part: call this before setComponentData.
     * For a KParts::MainWindow: call this before createGUI.
     */
    void setPluginLoadingMode(PluginLoadingMode loadingMode);

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
     */
    void setPluginInterfaceVersion(int version);

protected:
    PartBase(PartBasePrivate &dd);

    PartBasePrivate *d_ptr;

private:
    Q_DISABLE_COPY(PartBase)
};

} // namespace

#endif
