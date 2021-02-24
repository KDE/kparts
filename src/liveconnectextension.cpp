/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "liveconnectextension.h"

#include "readonlypart.h"

using namespace KParts;

class KParts::LiveConnectExtensionPrivate
{
};

LiveConnectExtension::LiveConnectExtension(KParts::ReadOnlyPart *parent)
    : QObject(parent)
    , d(nullptr)
{
}

LiveConnectExtension::~LiveConnectExtension()
{
}

bool LiveConnectExtension::get(const unsigned long, const QString &, Type &, unsigned long &, QString &)
{
    return false;
}

bool LiveConnectExtension::put(const unsigned long, const QString &, const QString &)
{
    return false;
}

bool LiveConnectExtension::call(const unsigned long, const QString &, const QStringList &, Type &, unsigned long &, QString &)
{
    return false;
}

void LiveConnectExtension::unregister(const unsigned long)
{
}

LiveConnectExtension *LiveConnectExtension::childObject(QObject *obj)
{
    return obj->findChild<KParts::LiveConnectExtension *>(QString(), Qt::FindDirectChildrenOnly);
}
