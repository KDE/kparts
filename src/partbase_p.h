/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999-2005 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef _KPARTS_PARTBASE_P_H
#define _KPARTS_PARTBASE_P_H

#include "partbase.h"

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
#include <KAboutData>
#endif

namespace KParts
{
class PartBasePrivate
{
public:
    Q_DECLARE_PUBLIC(PartBase)

    PartBasePrivate(PartBase *qq)
        : q_ptr(qq)
        , m_pluginLoadingMode(PartBase::LoadPlugins)
        , m_pluginInterfaceVersion(0)
        , m_obj(nullptr)
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
        , m_componentData(KAboutData::applicationData())
#endif
    {
    }

    virtual ~PartBasePrivate()
    {
    }

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
    // allows the KPart-subclasses to catch in its overridden method
    // the deprecated setting of KAboutData and update its KPluginMetaData to that
    virtual void setComponentData(const KAboutData &componentData)
    {
        m_componentData = componentData;
    }
    const KAboutData &componentData() const
    {
        return m_componentData;
    }
#endif

    PartBase *q_ptr;
    PartBase::PluginLoadingMode m_pluginLoadingMode;
    int m_pluginInterfaceVersion;
    QObject *m_obj;

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
private:
    KAboutData m_componentData;
#endif
};

} // namespace

#endif
