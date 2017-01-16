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

#include "liveconnectextension.h"

#include "readonlypart.h"

using namespace KParts;


LiveConnectExtension::LiveConnectExtension(KParts::ReadOnlyPart *parent)
    : QObject(parent), d(nullptr) {}

LiveConnectExtension::~LiveConnectExtension() {}

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

void LiveConnectExtension::unregister(const unsigned long) {}

LiveConnectExtension *LiveConnectExtension::childObject(QObject *obj)
{
    return obj->findChild<KParts::LiveConnectExtension *>(QString(), Qt::FindDirectChildrenOnly);
}
