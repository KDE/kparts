/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "openurlevent.h"
#include "event_p.h"

#include <QUrl>

using namespace KParts;

class KParts::OpenUrlEventPrivate : public KParts::EventPrivate
{
public:
    OpenUrlEventPrivate(const char *eventName, ReadOnlyPart *part, const QUrl &url, const OpenUrlArguments &args, const BrowserArguments &browserArgs)
        : EventPrivate(eventName)
        , m_part(part)
        , m_url(url)
        , m_args(args)
        , m_browserArgs(browserArgs)
    {
    }
    ~OpenUrlEventPrivate()
    {
    }
    static const char s_strOpenUrlEvent[];
    ReadOnlyPart *const m_part;
    const QUrl m_url;
    const OpenUrlArguments m_args;
    const BrowserArguments m_browserArgs;
};

const char KParts::OpenUrlEventPrivate::s_strOpenUrlEvent[] = "KParts/BrowserExtension/OpenURLevent";

OpenUrlEvent::OpenUrlEvent(ReadOnlyPart *part, const QUrl &url, const OpenUrlArguments &args, const BrowserArguments &browserArgs)
    : Event(*new OpenUrlEventPrivate(OpenUrlEventPrivate::s_strOpenUrlEvent, part, url, args, browserArgs))
{
}

OpenUrlEvent::~OpenUrlEvent() = default;

ReadOnlyPart *OpenUrlEvent::part() const
{
    Q_D(const OpenUrlEvent);

    return d->m_part;
}

QUrl OpenUrlEvent::url() const
{
    Q_D(const OpenUrlEvent);

    return d->m_url;
}

OpenUrlArguments OpenUrlEvent::arguments() const
{
    Q_D(const OpenUrlEvent);

    return d->m_args;
}

BrowserArguments OpenUrlEvent::browserArguments() const
{
    Q_D(const OpenUrlEvent);

    return d->m_browserArgs;
}

bool OpenUrlEvent::test(const QEvent *event)
{
    return Event::test(event, OpenUrlEventPrivate::s_strOpenUrlEvent);
}
