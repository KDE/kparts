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

#include "guiactivateevent.h"

using namespace KParts;

class KParts::GUIActivateEventPrivate
{
public:
    GUIActivateEventPrivate(bool activated)
        : m_bActivated(activated)
    {
    }
    static const char s_strGUIActivateEvent[];
    bool m_bActivated;
};

const char GUIActivateEventPrivate::s_strGUIActivateEvent[] = "KParts/GUIActivate";

GUIActivateEvent::GUIActivateEvent(bool activated) :
    Event(GUIActivateEventPrivate::s_strGUIActivateEvent),
    d(new GUIActivateEventPrivate(activated))
{
}

GUIActivateEvent::~GUIActivateEvent()
{
    delete d;
}

bool GUIActivateEvent::activated() const
{
    return d->m_bActivated;
}

bool GUIActivateEvent::test(const QEvent *event)
{
    return Event::test(event, GUIActivateEventPrivate::s_strGUIActivateEvent);
}
