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

#ifndef __kparts_browserhostextension_h__
#define __kparts_browserhostextension_h__

#include <kparts/kparts_export.h>

#include <QObject>

class QStringList;
class QString;
template<typename T> class QList;


namespace KParts
{

class ReadOnlyPart;
class OpenUrlArguments;
struct BrowserArguments;

/**
 * @class BrowserHostExtension browserhostextension.h <KParts/BrowserHostExtension>
 *
 * @short An extension class for container parts, i.e. parts that contain
 * other parts.
 * For instance a KHTMLPart hosts one part per frame.
 */
class KPARTS_EXPORT BrowserHostExtension : public QObject
{
    Q_OBJECT
public:
    BrowserHostExtension(KParts::ReadOnlyPart *parent);

    virtual ~BrowserHostExtension();

    /**
     * Returns a list of the names of all hosted child objects.
     *
     * Note that this method does not query the child objects recursively.
     */
    virtual QStringList frameNames() const;

    /**
     * Returns a list of pointers to all hosted child objects.
     *
     * Note that this method does not query the child objects recursively.
     */
    virtual const QList<KParts::ReadOnlyPart *> frames() const;

    /**
     * Returns the part that contains @p frame and that may be accessed
     * by @p callingPart
     */
    virtual BrowserHostExtension *findFrameParent(KParts::ReadOnlyPart *callingPart, const QString &frame);

    /**
     * Opens the given url in a hosted child frame. The frame name is specified in the
     * frameName variable in the @p browserArguments parameter (see KParts::BrowserArguments ) .
     */
    virtual bool openUrlInFrame(const QUrl &url,
                                const KParts::OpenUrlArguments &arguments,
                                const KParts::BrowserArguments &browserArguments);

    /**
     * Queries @p obj for a child object which inherits from this
     * BrowserHostExtension class. Convenience method.
     */
    static BrowserHostExtension *childObject(QObject *obj);

private:
    class BrowserHostExtensionPrivate;
    BrowserHostExtensionPrivate *const d;
};


}

#endif
