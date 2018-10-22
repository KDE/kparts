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
          m_bSelectable(true),
          m_autoDeleteWidget(true),
          m_autoDeletePart(true),
          m_manager(nullptr)
    {
    }

    ~PartPrivate()
    {
    }

    KIconLoader *m_iconLoader;
    bool m_bSelectable;
    bool m_autoDeleteWidget;
    bool m_autoDeletePart;
    PartManager *m_manager;
    QPointer<QWidget> m_widget;
};

} // namespace

#endif
