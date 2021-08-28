/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999-2005 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef _KPARTS_PART_P_H
#define _KPARTS_PART_P_H

#include "part.h"
#include "partbase_p.h"
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
#include "partmetadatautil_p.h"
#endif
// KF
#include <KPluginMetaData>
// Qt
#include <QPointer>
#include <QWidget>

namespace KParts
{
class PartPrivate : public PartBasePrivate
{
public:
    Q_DECLARE_PUBLIC(Part)

    explicit PartPrivate(Part *qq)
        : PartBasePrivate(qq)
        , m_iconLoader(nullptr)
        ,
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 72)
        m_bSelectable(true)
        ,
#endif
        m_autoDeleteWidget(true)
        , m_autoDeletePart(true)
        , m_manager(nullptr)
    {
    }

    ~PartPrivate()
    {
    }

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
    void setComponentData(const KAboutData &componentData) override
    {
        m_metaData = PartMetaDataUtil::fromKAboutData(componentData);
        PartBasePrivate::setComponentData(componentData);
    }
#endif

    KPluginMetaData m_metaData;
    KIconLoader *m_iconLoader;
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 72)
    bool m_bSelectable;
#endif
    bool m_autoDeleteWidget;
    bool m_autoDeletePart;
    PartManager *m_manager;
    QPointer<QWidget> m_widget;
};

} // namespace

#endif
