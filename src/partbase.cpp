/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
             (C) 1999-2005 David Faure <faure@kde.org>

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

PartBase::~PartBase()
{
    delete d_ptr;
}

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

KAboutData PartBase::componentData() const
{
    Q_D(const PartBase);
    return d->m_componentData;
}

void PartBase::setComponentData(const KAboutData &componentData)
{
    setComponentData(componentData, true);
}

void PartBase::setComponentData(const KAboutData &pluginData, bool bLoadPlugins)
{
    Q_D(PartBase);

    d->m_componentData = pluginData;
    KAboutData::registerPluginData(pluginData);
    KXMLGUIClient::setComponentName(pluginData.componentName(), pluginData.displayName());
    if (bLoadPlugins) {
        loadPlugins(d->m_obj, this, pluginData);
    }
}

void PartBase::loadPlugins(QObject *parent, KXMLGUIClient *parentGUIClient, const KAboutData &instance)
{
    Q_D(PartBase);

    if (d->m_pluginLoadingMode != DoNotLoadPlugins) {
        Plugin::loadPlugins(parent, parentGUIClient, instance.componentName(), d->m_pluginLoadingMode == LoadPlugins, d->m_pluginInterfaceVersion);
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

