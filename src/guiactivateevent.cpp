/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "guiactivateevent.h"
#include "event_p.h"

using namespace KParts;

class KParts::GUIActivateEventPrivate : public KParts::EventPrivate
{
public:
    GUIActivateEventPrivate(bool activated, const char *eventName)
        : EventPrivate(eventName)
        , m_bActivated(activated)
    {
    }
    static const char s_strGUIActivateEvent[];
    const bool m_bActivated;
};

const char GUIActivateEventPrivate::s_strGUIActivateEvent[] = "KParts/GUIActivate";

GUIActivateEvent::GUIActivateEvent(bool activated)
    : Event(*new GUIActivateEventPrivate(activated, GUIActivateEventPrivate::s_strGUIActivateEvent))
{
}

GUIActivateEvent::~GUIActivateEvent() = default;

bool GUIActivateEvent::activated() const
{
    Q_D(const GUIActivateEvent);

    return d->m_bActivated;
}

bool GUIActivateEvent::test(const QEvent *event)
{
    return Event::test(event, GUIActivateEventPrivate::s_strGUIActivateEvent);
}
