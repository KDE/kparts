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

#include "partselectevent.h"

using namespace KParts;


class KParts::PartSelectEventPrivate
{
public:
    PartSelectEventPrivate(bool selected,
                           Part *part,
                           QWidget *widget) :
        m_bSelected(selected),
        m_part(part),
        m_widget(widget)
    {
    }
    static const char s_strPartSelectEvent[];
    bool m_bSelected;
    Part *m_part;
    QWidget *m_widget;
};

const char PartSelectEventPrivate::s_strPartSelectEvent[] =
    "KParts/PartSelectEvent";

PartSelectEvent::PartSelectEvent(bool selected,
                                 Part *part,
                                 QWidget *widget) :
    Event(PartSelectEventPrivate::s_strPartSelectEvent),
    d(new PartSelectEventPrivate(selected, part, widget))
{
}

PartSelectEvent::~PartSelectEvent()
{
    delete d;
}

bool PartSelectEvent::selected() const
{
    return d->m_bSelected;
}

Part *PartSelectEvent::part() const
{
    return d->m_part;
}

QWidget *PartSelectEvent::widget() const
{
    return d->m_widget;
}

bool PartSelectEvent::test(const QEvent *event)
{
    return Event::test(event, PartSelectEventPrivate::s_strPartSelectEvent);
}
