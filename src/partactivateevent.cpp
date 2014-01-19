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

#include "partactivateevent.h"

using namespace KParts;

class KParts::PartActivateEventPrivate
{
public:
    PartActivateEventPrivate(bool activated,
                             Part *part,
                             QWidget *widget) :
        m_bActivated(activated),
        m_part(part),
        m_widget(widget)
    {
    }
    static const char *s_strPartActivateEvent;
    bool m_bActivated;
    Part *m_part;
    QWidget *m_widget;
};

const char *PartActivateEventPrivate::s_strPartActivateEvent = "KParts/PartActivateEvent";

PartActivateEvent::PartActivateEvent(bool activated,
                                     Part *part,
                                     QWidget *widget) :
    Event(PartActivateEventPrivate::s_strPartActivateEvent),
    d(new PartActivateEventPrivate(activated, part, widget))
{
}

PartActivateEvent::~PartActivateEvent()
{
    delete d;
}

bool PartActivateEvent::activated() const
{
    return d->m_bActivated;
}

Part *PartActivateEvent::part() const
{
    return d->m_part;
}

QWidget *PartActivateEvent::widget() const
{
    return d->m_widget;
}

bool PartActivateEvent::test(const QEvent *event)
{
    return Event::test(event, PartActivateEventPrivate::s_strPartActivateEvent);
}
