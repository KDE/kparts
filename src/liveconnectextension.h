/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef __kparts_liveconnectextension_h__
#define __kparts_liveconnectextension_h__

#include <kparts/kparts_export.h>

#include <QObject>
#include <memory>

template<class T1, class T2>
struct QPair;
template<typename T>
class QList;
class QStringList;
class QString;

namespace KParts
{
class ReadOnlyPart;
class LiveConnectExtensionPrivate;

/**
 * @class LiveConnectExtension liveconnectextension.h <KParts/LiveConnectExtension>
 *
 * @short An extension class for LiveConnect, i.e. a call from JavaScript
 * from a HTML page which embeds this part.
 * A part can have an object hierarchy by using objid as a reference
 * to an object.
 */
class KPARTS_EXPORT LiveConnectExtension : public QObject
{
    Q_OBJECT
public:
    enum Type {
        TypeVoid = 0,
        TypeBool,
        TypeFunction,
        TypeNumber,
        TypeObject,
        TypeString,
    };
    typedef QList<QPair<Type, QString>> ArgList;

    LiveConnectExtension(KParts::ReadOnlyPart *parent);

    virtual ~LiveConnectExtension();
    /**
     * get a field value from objid, return true on success
     */
    virtual bool get(const unsigned long objid, const QString &field, Type &type, unsigned long &retobjid, QString &value);
    /**
     * put a field value in objid, return true on success
     */
    virtual bool put(const unsigned long objid, const QString &field, const QString &value);
    /**
     * calls a function of objid, return true on success
     */
    virtual bool call(const unsigned long objid, const QString &func, const QStringList &args, Type &type, unsigned long &retobjid, QString &value);
    /**
     * notifies the part that there is no reference anymore to objid
     */
    virtual void unregister(const unsigned long objid);

    static LiveConnectExtension *childObject(QObject *obj);
Q_SIGNALS:
    /**
     * notify a event from the part of object objid
     */
    void partEvent(const unsigned long objid, const QString &event, const KParts::LiveConnectExtension::ArgList &args);

private:
    std::unique_ptr<LiveConnectExtensionPrivate> const d;
};

}

#endif
