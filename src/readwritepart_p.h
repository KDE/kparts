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

#ifndef _KPARTS_READWRITEPART_P_H
#define _KPARTS_READWRITEPART_P_H

#include "readonlypart_p.h"
#include "readwritepart.h"

#include <QEventLoop>


namespace KParts
{

class ReadWritePartPrivate: public ReadOnlyPartPrivate
{
public:
    Q_DECLARE_PUBLIC(ReadWritePart)

    explicit ReadWritePartPrivate(ReadWritePart *q): ReadOnlyPartPrivate(q)
    {
        m_bModified = false;
        m_bReadWrite = true;
        m_bClosing = false;
    }

    void _k_slotUploadFinished(KJob *job);

    void prepareSaving();

    bool m_bModified;
    bool m_bReadWrite;
    bool m_bClosing;
    QEventLoop m_eventLoop;
};

} // namespace

#endif
