/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
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
    const bool m_bSelected;
    Part *const m_part;
    QWidget *const m_widget;
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
