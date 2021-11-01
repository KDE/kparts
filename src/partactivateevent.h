/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    ~PartActivateEvent() override;
    bool activated() const;

    Part *part() const;
    QWidget *widget() const;

    static bool test(const QEvent *event);

private:
    Q_DECLARE_PRIVATE_D(Event::d, PartActivateEvent)
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 79)
    // Unused, kept for ABI compatibility
    const void *__kparts_d_do_not_use;
#endif
};

} // namespace

#endif
