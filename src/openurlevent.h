/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
                      David Faure <faure@kde.org>

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

#ifndef __kparts_openurlevent_h__
#define __kparts_openurlevent_h__

#include <kparts/event.h>
#include <kparts/openurlarguments.h>
#include <kparts/browserarguments.h>

class QUrl;

namespace KParts
{
class ReadOnlyPart;

/**
 * @class OpenUrlEvent openurlevent.h <KParts/OpenUrlEvent>
 *
 * @short The KParts::OpenUrlEvent event informs that a given part has opened a given URL.
 * Applications can use this event to send this information to interested plugins.
 *
 * The event should be sent before opening the URL in the part, so that the plugins
 * can use part()->url() to get the old URL.
 */
class KPARTS_EXPORT OpenUrlEvent : public Event
{
public:
    OpenUrlEvent(ReadOnlyPart *part, const QUrl &url,
                 const OpenUrlArguments &args = OpenUrlArguments(),
                 const BrowserArguments &browserArgs = BrowserArguments());
    virtual ~OpenUrlEvent();

    ReadOnlyPart *part() const;
    QUrl url() const;
    OpenUrlArguments arguments() const;
    BrowserArguments browserArguments() const;

    static bool test(const QEvent *event);

private:
    class OpenUrlEventPrivate;
    OpenUrlEventPrivate *const d;
};

}

#endif
