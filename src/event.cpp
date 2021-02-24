/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "event.h"
#include "event_p.h"

using namespace KParts;

// the answer!
#define KPARTS_EVENT_MAGIC 42

Event::Event(const char *eventName)
    : Event(*new EventPrivate(eventName))
{
}

Event::Event(EventPrivate &dd)
    : QEvent((QEvent::Type)(QEvent::User + KPARTS_EVENT_MAGIC))
    , d(&dd)
{
}

Event::~Event() = default;

const char *Event::eventName() const
{
    Q_D(const Event);

    return d->m_eventName;
}

bool Event::test(const QEvent *event)
{
    if (!event) {
        return false;
    }

    return (event->type() == (QEvent::Type)(QEvent::User + KPARTS_EVENT_MAGIC));
}

bool Event::test(const QEvent *event, const char *name)
{
    if (!test(event)) {
        return false;
    }

    return (strcmp(name, ((Event *)event)->eventName()) == 0);
}
