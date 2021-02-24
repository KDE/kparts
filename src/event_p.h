/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPARTS_EVENT_P_H
#define KPARTS_EVENT_P_H

namespace KParts
{
class EventPrivate
{
public:
    explicit EventPrivate(const char *eventName)
        : m_eventName(eventName)
    {
    }

    virtual ~EventPrivate() = default;

    const char *m_eventName;
};

}

#endif
