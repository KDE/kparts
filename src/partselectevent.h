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
#ifndef __kparts_partselectevent_h__
#define __kparts_partselectevent_h__

#include <kparts/event.h>

namespace KParts
{
class Part;

class PartSelectEventPrivate;
/**
 * @class PartSelectEvent partselectevent.h <KParts/PartSelectEvent>
 *
 * @short This event is sent when a part is selected or deselected.
 * @see KParts::PartManager::setSelectionPolicy
 */
class KPARTS_EXPORT PartSelectEvent : public Event
{
public:
    PartSelectEvent(bool selected, Part *part, QWidget *widget);
    virtual ~PartSelectEvent();
    bool selected() const;

    Part *part() const;
    QWidget *widget() const;

    static bool test(const QEvent *event);

private:
    PartSelectEventPrivate *const d;
};

} // namespace

#endif
