/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef __kparts_browserhostextension_h__
#define __kparts_browserhostextension_h__

#include <kparts/kparts_export.h>

#if KPARTS_ENABLE_DEPRECATED_SINCE(5, 78)

#include <QObject>
#include <memory>

class QStringList;
class QString;
template<typename T>
class QList;

namespace KParts
{
class ReadOnlyPart;
class OpenUrlArguments;
struct BrowserArguments;
class BrowserHostExtensionPrivate;

/**
 * @class BrowserHostExtension browserhostextension.h <KParts/BrowserHostExtension>
 *
 * @short An extension class for container parts, i.e. parts that contain
 * other parts.
 * For instance a KHTMLPart hosts one part per frame.
 * @deprecated since 5.78 for lack of usage
 */
class KPARTS_EXPORT BrowserHostExtension : public QObject
{
    Q_OBJECT
public:
    KPARTS_DEPRECATED_VERSION(5, 78, "Deprecated for lack of usage")
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
    virtual bool openUrlInFrame(const QUrl &url, const KParts::OpenUrlArguments &arguments, const KParts::BrowserArguments &browserArguments);

    /**
     * Queries @p obj for a child object which inherits from this
     * BrowserHostExtension class. Convenience method.
     */
    static BrowserHostExtension *childObject(QObject *obj);

private:
    std::unique_ptr<BrowserHostExtensionPrivate> const d;
};

}

#endif
#endif
