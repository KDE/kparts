/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "partactivateevent.h"
#include "event_p.h"

using namespace KParts;

class KParts::PartActivateEventPrivate : public KParts::EventPrivate
{
public:
    PartActivateEventPrivate(const char *eventName, bool activated, Part *part, QWidget *widget)
        : EventPrivate(eventName)
        , m_bActivated(activated)
        , m_part(part)
        , m_widget(widget)
    {
    }
    static const char s_strPartActivateEvent[];
    const bool m_bActivated;
    Part *const m_part;
    QWidget *const m_widget;
};

const char PartActivateEventPrivate::s_strPartActivateEvent[] = "KParts/PartActivateEvent";

PartActivateEvent::PartActivateEvent(bool activated, Part *part, QWidget *widget)
    : Event(*new PartActivateEventPrivate(PartActivateEventPrivate::s_strPartActivateEvent, activated, part, widget))
{
}

PartActivateEvent::~PartActivateEvent() = default;

bool PartActivateEvent::activated() const
{
    Q_D(const PartActivateEvent);

    return d->m_bActivated;
}

Part *PartActivateEvent::part() const
{
    Q_D(const PartActivateEvent);

    return d->m_part;
}

QWidget *PartActivateEvent::widget() const
{
    Q_D(const PartActivateEvent);

    return d->m_widget;
}

bool PartActivateEvent::test(const QEvent *event)
{
    return Event::test(event, PartActivateEventPrivate::s_strPartActivateEvent);
}
