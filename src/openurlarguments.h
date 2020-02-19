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

#ifndef _KPARTS_OPENURLARGUMENTS_H
#define _KPARTS_OPENURLARGUMENTS_H

#include <kparts/kparts_export.h>

#include <QSharedDataPointer>
#include <QString>
#include <QMap>


namespace KParts
{


class OpenUrlArgumentsPrivate;

/**
 * @class OpenUrlArguments openurlarguments.h <KParts/OpenUrlArguments>
 *
 * @short OpenUrlArguments is the set of arguments that specify
 * how a URL should be opened by KParts::ReadOnlyPart::openUrl().
 *
 * For instance reload() indicates that the url should be loaded
 * from the network even if it matches the current url of the part.
 *
 * All setter methods in this class are for the class that calls openUrl
 * (usually the hosting application), all the getter methods are for the part.
 */
class KPARTS_EXPORT OpenUrlArguments
{
public:
    OpenUrlArguments();
    OpenUrlArguments(const OpenUrlArguments &other);
    OpenUrlArguments &operator=(const OpenUrlArguments &other);
    ~OpenUrlArguments();

    /**
     * @return true to indicate that the part should reload the URL,
     * i.e. the cache shouldn't be used (forced reload).
     */
    bool reload() const;
    /**
     * Indicates that the url should be loaded
     * from the network even if it matches the current url of the part.
     */
    void setReload(bool b);

    /**
     * xOffset is the horizontal scrolling of the part's widget
     * (in case it's a scrollview). This is saved into the history
     * and restored when going back in the history.
     */
    int xOffset() const;
    void setXOffset(int x);

    /**
     * yOffset is the vertical scrolling of the part's widget
     * (in case it's a scrollview). This is saved into the history
     * and restored when going back in the history.
     */
    int yOffset() const;
    void setYOffset(int y);

    /**
     * The mimetype to use when opening the url, when known by the calling application.
     */
    QString mimeType() const;
    void setMimeType(const QString &mime);

    /**
     * True if the user requested that the URL be opened.
     * False if the URL should be opened due to an external event, like javascript popups
     * or automatic redirections.
     * This is true by default
     * @since 4.1
     */
    bool actionRequestedByUser() const;
    void setActionRequestedByUser(bool userRequested);

    /**
     * Meta-data to associate with the KIO operation that will be used to open the URL.
     * This method can be used to add or retrieve metadata.
     * @see KIO::TransferJob etc.
     */
    QMap<QString, QString> &metaData();
    const QMap<QString, QString> &metaData() const;

private:
    QSharedDataPointer<OpenUrlArgumentsPrivate> d;
};

} // namespace

#endif
