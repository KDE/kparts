/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
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
    const bool m_bActivated;
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
