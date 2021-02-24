/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef __kparts_openurlevent_h__
#define __kparts_openurlevent_h__

#include <kparts/browserarguments.h>
#include <kparts/event.h>
#include <kparts/openurlarguments.h>

class QUrl;

namespace KParts
{
class ReadOnlyPart;
class OpenUrlEventPrivate;

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
    OpenUrlEvent(ReadOnlyPart *part,
                 const QUrl &url,
                 const OpenUrlArguments &args = OpenUrlArguments(),
                 const BrowserArguments &browserArgs = BrowserArguments());
    virtual ~OpenUrlEvent();

    ReadOnlyPart *part() const;
    QUrl url() const;
    OpenUrlArguments arguments() const;
    BrowserArguments browserArguments() const;

    static bool test(const QEvent *event);

private:
    Q_DECLARE_PRIVATE_D(Event::d, OpenUrlEvent)
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 79)
    // Unused, kept for ABI compatibility
    const void *__kparts_d_do_not_use;
#endif
};

}

#endif
