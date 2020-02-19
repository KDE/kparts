/* This file is part of the KDE project
   Copyright (C) 2010 David Faure <faure@kde.org>

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

#ifndef KPARTS_HTMLEXTENSION_H
#define KPARTS_HTMLEXTENSION_H

#include <kparts/kparts_export.h>

#include <QObject>

class QUrl;

namespace KParts
{

class ReadOnlyPart;
class HtmlExtensionPrivate;

/**
 * @class HtmlExtension htmlextension.h <KParts/HtmlExtension>
 *
 * @short An extension for KParts to provide HTML-related features
 *
 * Use qobject_cast to cast the extension to interesting interfaces, like
 * qobject_cast<KParts::SelectorInterface>.
 *
 * @since 4.6
 */
class KPARTS_EXPORT HtmlExtension : public QObject
{
    Q_OBJECT
public:
    explicit HtmlExtension(KParts::ReadOnlyPart *parent);
    ~HtmlExtension();

    /**
     * Queries @p obj for a child object which inherits from this
     * HtmlExtension class.
     */
    static HtmlExtension *childObject(QObject *obj);

    /**
     * Returns the current base url of the part that implements this extension.
     *
     * This function is mostly used to resolve any relative URLs that might be
     * returned when querying the part for links.
     */
    virtual QUrl baseUrl() const = 0;

    /**
     * Returns true if portions of the content in the part that implements
     * this extension are selected.
     *
     * By default this function returns false.
     */
    virtual bool hasSelection() const;

private:
    // for future extensions
    HtmlExtensionPrivate *const d;
};

} // namespace KParts

#endif /* KPARTS_HTMLEXTENSION_H */
