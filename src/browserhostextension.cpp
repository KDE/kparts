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

#include "browserhostextension.h"

#include "readonlypart.h"

using namespace KParts;


namespace KParts
{

class BrowserHostExtension::BrowserHostExtensionPrivate
{
public:
    BrowserHostExtensionPrivate()
    {
    }
    ~BrowserHostExtensionPrivate()
    {
    }

    KParts::ReadOnlyPart *m_part;
};

}

BrowserHostExtension::BrowserHostExtension(KParts::ReadOnlyPart *parent)
    : QObject(parent), d(new BrowserHostExtensionPrivate)
{
    d->m_part = parent;
}

BrowserHostExtension::~BrowserHostExtension()
{
    delete d;
}

QStringList BrowserHostExtension::frameNames() const
{
    return QStringList();
}

const QList<KParts::ReadOnlyPart *> BrowserHostExtension::frames() const
{
    return QList<KParts::ReadOnlyPart *>();
}

bool BrowserHostExtension::openUrlInFrame(const QUrl &,
        const KParts::OpenUrlArguments &,
        const KParts::BrowserArguments &)
{
    return false;
}

BrowserHostExtension *BrowserHostExtension::childObject(QObject *obj)
{
    return obj->findChild<KParts::BrowserHostExtension *>(QString(), Qt::FindDirectChildrenOnly);
}

BrowserHostExtension *
BrowserHostExtension::findFrameParent(KParts::ReadOnlyPart *callingPart, const QString &frame)
{
    Q_UNUSED(callingPart);
    Q_UNUSED(frame);
    return 0;
}
