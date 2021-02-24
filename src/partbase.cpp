/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999-2005 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "partbase.h"
#include "partbase_p.h"

#include "plugin.h"

using namespace KParts;

PartBase::PartBase()
    : d_ptr(new PartBasePrivate(this))
{
}

PartBase::PartBase(PartBasePrivate &dd)
    : d_ptr(&dd)
{
}

PartBase::~PartBase() = default;

void PartBase::setPartObject(QObject *obj)
{
    Q_D(PartBase);

    d->m_obj = obj;
}

QObject *PartBase::partObject() const
{
    Q_D(const PartBase);

    return d->m_obj;
}

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
KAboutData PartBase::componentData() const
{
    Q_D(const PartBase);

    return d->componentData();
}
#endif

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
void PartBase::setComponentData(const KAboutData &componentData)
{
    setComponentData(componentData, true);
}
#endif

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
void PartBase::setComponentData(const KAboutData &pluginData, bool bLoadPlugins)
{
    Q_D(PartBase);

    d->setComponentData(pluginData);

    // backward-compatible registration, usage deprecated
#if KCOREADDONS_BUILD_DEPRECATED_SINCE(5, 76)
    QT_WARNING_PUSH
    QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
    QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
    KAboutData::registerPluginData(pluginData);
    QT_WARNING_POP
#endif

    KXMLGUIClient::setComponentName(pluginData.componentName(), pluginData.displayName());
    if (bLoadPlugins) {
        loadPlugins(d->m_obj, this, pluginData.componentName());
    }
}
#endif

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
void PartBase::loadPlugins(QObject *parent, KXMLGUIClient *parentGUIClient, const KAboutData &instance)
{
    loadPlugins(parent, parentGUIClient, instance.componentName());
}
#endif

void PartBase::loadPlugins(QObject *parent, KXMLGUIClient *parentGuiClient, const QString &parentInstanceName)
{
    Q_D(PartBase);

    if (d->m_pluginLoadingMode != DoNotLoadPlugins) {
        Plugin::loadPlugins(parent, parentGuiClient, parentInstanceName, d->m_pluginLoadingMode == LoadPlugins, d->m_pluginInterfaceVersion);
    }
}

void PartBase::setPluginLoadingMode(PluginLoadingMode loadingMode)
{
    Q_D(PartBase);

    d->m_pluginLoadingMode = loadingMode;
}

void KParts::PartBase::setPluginInterfaceVersion(int version)
{
    Q_D(PartBase);

    d->m_pluginInterfaceVersion = version;
}
