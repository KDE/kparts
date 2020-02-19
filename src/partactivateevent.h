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
#ifndef __kparts_partactivateevent_h__
#define __kparts_partactivateevent_h__

#include <kparts/event.h>

namespace KParts
{
class Part;

class PartActivateEventPrivate;
/**
 * @class PartActivateEvent partactivateevent.h <KParts/PartActivateEvent>
 *
 * @short This event is sent by the part manager when the active part changes.
 * Each time the active part changes, it will send first a PartActivateEvent
 * with activated=false, part=oldActivePart, widget=oldActiveWidget
 * and then another PartActivateEvent
 * with activated=true, part=newPart, widget=newWidget.
 * @see KParts::Part::partActivateEvent
 */
class KPARTS_EXPORT PartActivateEvent : public Event
{
public:
    PartActivateEvent(bool activated, Part *part, QWidget *widget);
    virtual ~PartActivateEvent();
    bool activated() const;

    Part *part() const;
    QWidget *widget() const;

    static bool test(const QEvent *event);

private:
    PartActivateEventPrivate *const d;
};

} // namespace

#endif
