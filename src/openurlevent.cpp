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

