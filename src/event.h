/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef __kparts_event_h__
#define __kparts_event_h__

#include <kparts/kparts_export.h>

#include <QEvent>

class QWidget;

namespace KParts
{

class EventPrivate;
/**
 * @class Event event.h <KParts/Event>
 *
 * @short Base class for all KParts events.
 */
class KPARTS_EXPORT Event : public QEvent
{
public:
    Event(const char *eventName);
    virtual ~Event();
    virtual const char *eventName() const;

    static bool test(const QEvent *event);
    static bool test(const QEvent *event, const char *name);

private:
    EventPrivate *const d;
};

} // namespace

#endif
