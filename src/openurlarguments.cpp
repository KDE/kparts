/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
             (C) 1999-2005 David Faure <faure@kde.org>

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

#include "openurlarguments.h"


class KParts::OpenUrlArgumentsPrivate : public QSharedData
{
public:
    OpenUrlArgumentsPrivate()
        : reload(false),
          actionRequestedByUser(true),
          xOffset(0),
          yOffset(0),
          mimeType(),
          metaData()
    {}
    bool reload;
    bool actionRequestedByUser;
    int xOffset;
    int yOffset;
    QString mimeType;
    QMap<QString, QString> metaData;
};

KParts::OpenUrlArguments::OpenUrlArguments()
    : d(new OpenUrlArgumentsPrivate)
{
}

KParts::OpenUrlArguments::OpenUrlArguments(const OpenUrlArguments &other)
    : d(other.d)
{
}

KParts::OpenUrlArguments &KParts::OpenUrlArguments::operator=(const OpenUrlArguments &other)
{
    d = other.d;
    return *this;
}

KParts::OpenUrlArguments::~OpenUrlArguments()
{
}

bool KParts::OpenUrlArguments::reload() const
{
    return d->reload;
}

void KParts::OpenUrlArguments::setReload(bool b)
{
    d->reload = b;
}

int KParts::OpenUrlArguments::xOffset() const
{
    return d->xOffset;
}

void KParts::OpenUrlArguments::setXOffset(int x)
{
    d->xOffset = x;
}

int KParts::OpenUrlArguments::yOffset() const
{
    return d->yOffset;
}

void KParts::OpenUrlArguments::setYOffset(int y)
{
    d->yOffset = y;
}

QString KParts::OpenUrlArguments::mimeType() const
{
    return d->mimeType;
}

void KParts::OpenUrlArguments::setMimeType(const QString &mime)
{
    d->mimeType = mime;
}

QMap<QString, QString> &KParts::OpenUrlArguments::metaData()
{
    return d->metaData;
}

const QMap<QString, QString> &KParts::OpenUrlArguments::metaData() const
{
    return d->metaData;
}

bool KParts::OpenUrlArguments::actionRequestedByUser() const
{
    return d->actionRequestedByUser;
}

void KParts::OpenUrlArguments::setActionRequestedByUser(bool userRequested)
{
    d->actionRequestedByUser = userRequested;
}
