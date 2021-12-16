/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugin.h"

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 90)

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")

#include "part.h"

#include <KConfigGroup>
#include <KDesktopFile>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KPluginLoader>
#include <KPluginMetaData>
#include <KSharedConfig>
#include <KXMLGUIFactory>
#if !KPARTS_BUILD_DEPRECATED_SINCE(5, 77) || KCOREADDONS_BUILD_DEPRECATED_SINCE(5, 76)
#include <KAboutData>
#endif

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

using namespace KParts;

class KParts::PluginPrivate
{
public:
    QString m_parentInstance;
    QString m_library; // filename of the library
};

Plugin::Plugin(QObject *parent)
    : QObject(parent)
    , d(new PluginPrivate())
{
    // qDebug() << className();
}

Plugin::~Plugin() = default;

QString Plugin::xmlFile() const
{
    QString path = KXMLGUIClient::xmlFile();

    if (d->m_parentInstance.isEmpty() || (!path.isEmpty() && QDir::isAbsolutePath(path))) {
        return path;
    }

    QString absPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, d->m_parentInstance + QLatin1Char('/') + path);
    Q_ASSERT(!absPath.isEmpty());
    return absPath;
}

QString Plugin::localXMLFile() const
{
    QString path = KXMLGUIClient::xmlFile();

    if (d->m_parentInstance.isEmpty() || (!path.isEmpty() && QDir::isAbsolutePath(path))) {
        return path;
    }

    QString absPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + d->m_parentInstance + QLatin1Char('/') + path;
    return absPath;
}

// static
QList<Plugin::PluginInfo> Plugin::pluginInfos(const QString &componentName)
{
    QList<PluginInfo> plugins;

    QMap<QString, QStringList> sortedPlugins;

    const QStringList dirs =
        QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, componentName + QLatin1String("/kpartplugins"), QStandardPaths::LocateDirectory);
    for (const QString &dir : dirs) {
        const auto rcfiles = QDir(dir).entryList(QStringList(QStringLiteral("*.rc")));
        for (const QString &file : rcfiles) {
            const QFileInfo fInfo(dir + QLatin1Char('/') + file);
            QMap<QString, QStringList>::Iterator mapIt = sortedPlugins.find(fInfo.fileName());
            if (mapIt == sortedPlugins.end()) {
                mapIt = sortedPlugins.insert(fInfo.fileName(), QStringList());
            }
            mapIt.value().append(fInfo.absoluteFilePath());
        }
    }

    QMap<QString, QStringList>::ConstIterator mapIt = sortedPlugins.constBegin();
    QMap<QString, QStringList>::ConstIterator mapEnd = sortedPlugins.constEnd();
    for (; mapIt != mapEnd; ++mapIt) {
        PluginInfo info;
        QString doc;
        info.m_absXMLFileName = KXMLGUIClient::findMostRecentXMLFile(mapIt.value(), doc);
        if (info.m_absXMLFileName.isEmpty()) {
            continue;
        }

        // qDebug() << "found KParts Plugin : " << info.m_absXMLFileName;
        info.m_relXMLFileName = QLatin1String("kpartplugins/") + mapIt.key();

        info.m_document.setContent(doc);
        if (info.m_document.documentElement().isNull()) {
            continue;
        }

        plugins.append(info);
    }

    return plugins;
}

void Plugin::loadPlugins(QObject *parent, const QString &componentName)
{
    loadPlugins(parent, pluginInfos(componentName), componentName);
}

void Plugin::loadPlugins(QObject *parent, const QList<PluginInfo> &pluginInfos, const QString &componentName)
{
    for (const auto &pluginInfo : pluginInfos) {
        const QString library = pluginInfo.m_document.documentElement().attribute(QStringLiteral("library"));

        if (library.isEmpty() || hasPlugin(parent, library)) {
            continue;
        }

        Plugin *plugin = loadPlugin(parent, library, pluginInfo.m_document.documentElement().attribute(QStringLiteral("X-KDE-PluginKeyword")));

        if (plugin) {
            plugin->d->m_parentInstance = componentName;
            plugin->setXMLFile(pluginInfo.m_relXMLFileName, false, false);
            plugin->setDOMDocument(pluginInfo.m_document);
        }
    }
}

void Plugin::loadPlugins(QObject *parent, const QList<PluginInfo> &pluginInfos)
{
    loadPlugins(parent, pluginInfos, QString());
}

// static
Plugin *Plugin::loadPlugin(QObject *parent, const QString &libname, const QString &keyword)
{
    KPluginLoader loader(libname);
    KPluginFactory *factory = loader.factory();

    if (!factory) {
        return nullptr;
    }

    Plugin *plugin = factory->create<Plugin>(keyword, parent);
    if (!plugin) {
        return nullptr;
    }
    plugin->d->m_library = libname;
    return plugin;
}

QList<KParts::Plugin *> Plugin::pluginObjects(QObject *parent)
{
    QList<KParts::Plugin *> objects;

    if (!parent) {
        return objects;
    }

    objects = parent->findChildren<Plugin *>(QString(), Qt::FindDirectChildrenOnly);
    return objects;
}

bool Plugin::hasPlugin(QObject *parent, const QString &library)
{
    const QObjectList plugins = parent->children();

    return std::any_of(plugins.begin(), plugins.end(), [&library](QObject *p) {
        Plugin *plugin = qobject_cast<Plugin *>(p);
        return (plugin && plugin->d->m_library == library);
    });
}

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
void Plugin::setComponentData(const KAboutData &pluginData)
{
    // backward-compatible registration, usage deprecated
#if KCOREADDONS_BUILD_DEPRECATED_SINCE(5, 76)
    QT_WARNING_PUSH
    QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
    QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
    KAboutData::registerPluginData(pluginData);
    QT_WARNING_POP
#endif

    KXMLGUIClient::setComponentName(pluginData.componentName(), pluginData.displayName());
}
#endif

void Plugin::setMetaData(const KPluginMetaData &metaData)
{
    // backward-compatible registration, usage deprecated
#if KCOREADDONS_BUILD_DEPRECATED_SINCE(5, 76)
    QT_WARNING_PUSH
    QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
    QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
    KAboutData::registerPluginData(KAboutData::fromPluginMetaData(metaData));
    QT_WARNING_POP
#endif

    KXMLGUIClient::setComponentName(metaData.pluginId(), metaData.name());
}

void Plugin::loadPlugins(QObject *parent,
                         KXMLGUIClient *parentGUIClient,
                         const QString &componentName,
                         bool enableNewPluginsByDefault,
                         int interfaceVersionRequired)
{
    KConfigGroup cfgGroup(KSharedConfig::openConfig(componentName + QLatin1String("rc")), "KParts Plugins");
    const QList<PluginInfo> plugins = pluginInfos(componentName);
    for (const auto &pluginInfo : plugins) {
        QDomElement docElem = pluginInfo.m_document.documentElement();
        QString library = docElem.attribute(QStringLiteral("library"));
        QString keyword;

        if (library.isEmpty()) {
            continue;
        }

        // Check configuration
        const QString name = docElem.attribute(QStringLiteral("name"));

        bool pluginEnabled = enableNewPluginsByDefault;
        if (cfgGroup.hasKey(name + QLatin1String("Enabled"))) {
            pluginEnabled = cfgGroup.readEntry(name + QLatin1String("Enabled"), false);
        } else { // no user-setting, load plugin default setting
            QString relPath = componentName + QLatin1Char('/') + pluginInfo.m_relXMLFileName;
            relPath.truncate(relPath.lastIndexOf(QLatin1Char('.'))); // remove extension
            relPath += QLatin1String(".desktop");
            // qDebug() << "looking for " << relPath;
            const QString desktopfile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, relPath);
            if (!desktopfile.isEmpty()) {
                // qDebug() << "loadPlugins found desktop file for " << name << ": " << desktopfile;
                KDesktopFile _desktop(desktopfile);
                const KConfigGroup desktop = _desktop.desktopGroup();
                keyword = desktop.readEntry("X-KDE-PluginKeyword", "");
                pluginEnabled = desktop.readEntry("X-KDE-PluginInfo-EnabledByDefault", enableNewPluginsByDefault);
                if (interfaceVersionRequired != 0) {
                    const int version = desktop.readEntry("X-KDE-InterfaceVersion", 1);
                    if (version != interfaceVersionRequired) {
                        // qDebug() << "Discarding plugin " << name << ", interface version " << version << ", expected " << interfaceVersionRequired;
                        pluginEnabled = false;
                    }
                }
            } else {
                // qDebug() << "loadPlugins no desktop file found in " << relPath;
            }
        }

        // search through already present plugins
        const QObjectList pluginList = parent->children();

        bool pluginFound = false;
        for (auto *p : pluginList) {
            Plugin *plugin = qobject_cast<Plugin *>(p);
            if (plugin && plugin->d->m_library == library) {
                // delete and unload disabled plugins
                if (!pluginEnabled) {
                    // qDebug() << "remove plugin " << name;
                    KXMLGUIFactory *factory = plugin->factory();
                    if (factory) {
                        factory->removeClient(plugin);
                    }
                    delete plugin;
                }

                pluginFound = true;
                break;
            }
        }

        // if the plugin is already loaded or if it's disabled in the
        // configuration do nothing
        if (pluginFound || !pluginEnabled) {
            continue;
        }

        // qDebug() << "load plugin " << name << " " << library << " " << keyword;
        Plugin *plugin = loadPlugin(parent, library, keyword);

        if (plugin) {
            plugin->d->m_parentInstance = componentName;
            plugin->setXMLFile(pluginInfo.m_relXMLFileName, false, false);
            plugin->setDOMDocument(pluginInfo.m_document);
            parentGUIClient->insertChildClient(plugin);
        }
    }
}
#endif
