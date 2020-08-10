/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999-2005 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
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
