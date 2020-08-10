/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999-2005 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef _KPARTS_PART_P_H
#define _KPARTS_PART_P_H

#include "partbase_p.h"
#include "part.h"

#include <QWidget>
#include <QPointer>


namespace KParts
{

class PartPrivate: public PartBasePrivate
{
public:
    Q_DECLARE_PUBLIC(Part)

    explicit PartPrivate(Part *q)
        : PartBasePrivate(q),
          m_iconLoader(nullptr),
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 72)
          m_bSelectable(true),
#endif
          m_autoDeleteWidget(true),
          m_autoDeletePart(true),
          m_manager(nullptr)
    {
    }

    ~PartPrivate()
    {
    }

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
