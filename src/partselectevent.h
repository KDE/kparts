/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef __kparts_partselectevent_h__
#define __kparts_partselectevent_h__

#include <kparts/event.h>

#if KPARTS_ENABLE_DEPRECATED_SINCE(5, 103)
namespace KParts
{
class Part;

class PartSelectEventPrivate;
/**
 * @class PartSelectEvent partselectevent.h <KParts/PartSelectEvent>
 *
 * @short This event is sent when a part is selected or deselected.
 * @see KParts::PartManager::setSelectionPolicy
 * @deprecated Since 5.72, for lack of usage.
 */
class KPARTS_EXPORT PartSelectEvent : public Event
{
public:
    KPARTS_DEPRECATED_VERSION_BELATED(5, 103, 5, 72, "For lack of usage.")
    PartSelectEvent(bool selected, Part *part, QWidget *widget);
    ~PartSelectEvent() override;
    bool selected() const;

    Part *part() const;
    QWidget *widget() const;

    KPARTS_DEPRECATED_VERSION_BELATED(5, 103, 5, 72, "For lack of usage.")
    static bool test(const QEvent *event);

private:
    Q_DECLARE_PRIVATE_D(Event::d, PartSelectEvent)
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 79)
    // Unused, kept for ABI compatibility
    const void *__kparts_d_do_not_use;
#endif
};

} // namespace

#endif
#endif
