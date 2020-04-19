/* This file is part of the KDE project
   Copyright (C) 2012 Dawit Alemayehu <adawit@kde.org>

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

#ifndef KPARTS_LISTINGNOTIFICATIONEXTENSION_H
#define KPARTS_LISTINGNOTIFICATIONEXTENSION_H

#include <kparts/kparts_export.h>

#include <QObject>

class KFileItemList;

namespace KParts
{

class ReadOnlyPart;

/**
 * @class ListingNotificationExtension listingnotificationextension.h <KParts/ListingNotificationExtension>
 *
 * @short An extension for receiving listing change notification.
 *
 * This extension is intended for implementation by parts that provide listing
 * services, e.g. file management and is intended to notify about changes to
 * a given listing. For example, if file management part implemented this extension
 * it would emit @ref itemsDeleted and @ref itemsAdded signal whenever new files
 * or folders are deleted and added to a directory respectively.
 *
 * @since 4.9.2
 */
class KPARTS_EXPORT ListingNotificationExtension : public QObject
{
    Q_OBJECT

public:
    /**
     * Supported notification event types.
     * @see NotificationEventTypes
     */
    enum NotificationEventType {
        None = 0x00,
        ItemsAdded = 0x01,      /*!< New items added to the listing. */
        ItemsDeleted = 0x02     /*!< Items deleted from the listing. */
    };

    /**
     * Stores a combination of #NotificationEventType values.
     */
    Q_DECLARE_FLAGS(NotificationEventTypes, NotificationEventType)

    /*! Constructor */
    ListingNotificationExtension(KParts::ReadOnlyPart *parent);

    /*! Destructor */
    virtual ~ListingNotificationExtension();

    /**
     * Returns the OR'ed value of the notification types supported by the part
     * that implements this extension.
     *
     * By default this function returns None.
     */
    virtual NotificationEventTypes supportedNotificationEventTypes() const;

    /**
      * Queries @p obj for a child object which inherits from this class.
      */
    static ListingNotificationExtension *childObject(QObject *obj);

Q_SIGNALS:
    /**
      * This signal is emitted when one of the notification events listed
      * in @ref NotificationEventType occur.
      */
    void listingEvent(KParts::ListingNotificationExtension::NotificationEventType, const KFileItemList &);

private:
    class ListingNotificationExtensionPrivate;
    ListingNotificationExtension *const d;
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(KParts::ListingNotificationExtension::NotificationEventTypes)

#endif /* KPARTS_LISTINGNOTIFICATIONEXTENSION_H */
