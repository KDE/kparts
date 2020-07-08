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

#ifndef _KPARTS_PARTBASE_P_H
#define _KPARTS_PARTBASE_P_H

#include "partbase.h"

#include <KAboutData>

namespace KParts
{

class PartBasePrivate
{
public:
    Q_DECLARE_PUBLIC(PartBase)

    PartBasePrivate(PartBase *q): q_ptr(q),
        m_pluginLoadingMode(PartBase::LoadPlugins),
        m_pluginInterfaceVersion(0),
        m_obj(nullptr),
        m_componentData(KAboutData::applicationData())
    {
    }

    virtual ~PartBasePrivate()
    {
    }

    PartBase *q_ptr;
    PartBase::PluginLoadingMode m_pluginLoadingMode;
    int m_pluginInterfaceVersion;
    QObject *m_obj;
    KAboutData m_componentData;
};

} // namespace

#endif
