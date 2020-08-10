/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "event.h"

using namespace KParts;

//the answer!
#define KPARTS_EVENT_MAGIC 42

class KParts::EventPrivate
{
public:
    EventPrivate(const char *eventName) :
        m_eventName(eventName)
    {
    }
    const char *m_eventName;
};

Event::Event(const char *eventName)
    : QEvent((QEvent::Type)(QEvent::User + KPARTS_EVENT_MAGIC))
    , d(new EventPrivate(eventName))
{
}

Event::~Event()
{
    delete d;
}

const char *Event::eventName() const
{
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
