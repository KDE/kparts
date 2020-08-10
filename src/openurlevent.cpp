/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "openurlevent.h"

#include <QUrl>

using namespace KParts;


class Q_DECL_HIDDEN OpenUrlEvent::OpenUrlEventPrivate
{
public:
    OpenUrlEventPrivate(ReadOnlyPart *part,
                        const QUrl &url,
                        const OpenUrlArguments &args,
                        const BrowserArguments &browserArgs)
        : m_part(part)
        , m_url(url)
        , m_args(args)
        , m_browserArgs(browserArgs)
    {
    }
    ~OpenUrlEventPrivate()
    {
    }
    static const char s_strOpenUrlEvent[];
    ReadOnlyPart *m_part;
    QUrl m_url;
    OpenUrlArguments m_args;
    BrowserArguments m_browserArgs;
};

const char OpenUrlEvent::OpenUrlEventPrivate::s_strOpenUrlEvent[] =
    "KParts/BrowserExtension/OpenURLevent";


OpenUrlEvent::OpenUrlEvent(ReadOnlyPart *part, const QUrl &url,
                           const OpenUrlArguments &args,
                           const BrowserArguments &browserArgs)
    : Event(OpenUrlEventPrivate::s_strOpenUrlEvent)
    , d(new OpenUrlEventPrivate(part, url, args, browserArgs))
{
}

OpenUrlEvent::~OpenUrlEvent()
{
    delete d;
}

ReadOnlyPart *OpenUrlEvent::part() const
{
    return d->m_part;
}

QUrl OpenUrlEvent::url() const
{
    return d->m_url;
}

OpenUrlArguments OpenUrlEvent::arguments() const
{
    return d->m_args;
}

BrowserArguments OpenUrlEvent::browserArguments() const
{
    return d->m_browserArgs;
}

bool OpenUrlEvent::test(const QEvent *event)
{
    return Event::test(event, OpenUrlEventPrivate::s_strOpenUrlEvent);
}

