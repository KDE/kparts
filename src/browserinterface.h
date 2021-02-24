/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2001 Simon Hausmann <hausmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPARTS_BROWSERINTERFACE_H
#define KPARTS_BROWSERINTERFACE_H

#include <kparts/kparts_export.h>

#include <QObject>

class QVariant;

namespace KParts
{
/**
 * @class BrowserInterface browserinterface.h <KParts/BrowserInterface>
 *
 * @short The purpose of this interface is to allow a direct communication between
 * a KPart and the hosting browser shell (for example Konqueror) . A
 * shell implementing this interface can propagate it to embedded kpart
 * components by using the setBrowserInterface call of the part's
 * KParts::BrowserExtension object.
 *
 * This interface looks not very rich, but the main functionality is
 * implemented using the callMethod method for part->shell
 * communication and using Qt properties for allowing a part to
 * to explicitly query information from the shell.
 *
 * Konqueror in particular, as 'reference' implementation, provides
 * the following functionality through this interface:
 *
 * Qt properties:
 *   Q_PROPERTY( uint historyLength READ historyLength );
 *
 * Callable methods:
 *       void goHistory( int );
 *
 */
class KPARTS_EXPORT BrowserInterface : public QObject
{
    Q_OBJECT
public:
    explicit BrowserInterface(QObject *parent);
    virtual ~BrowserInterface();

    /**
     * Perform a dynamic invocation of a method in the BrowserInterface
     * implementation. Methods are to be implemented as simple Qt slots.
     * You should only include the method name, and not the signature,
     * in the name argument.
     */
    void callMethod(const char *name, const QVariant &argument);
};

}

#endif
