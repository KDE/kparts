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

#ifndef _KPARTS_READONLYPART_P_H
#define _KPARTS_READONLYPART_P_H

#include "part_p.h"
#include "readonlypart.h"
#include "openurlarguments.h"

namespace KIO
{
class FileCopyJob;
class StatJob;
}

namespace KParts
{

class ReadOnlyPartPrivate: public PartPrivate
{
public:
    Q_DECLARE_PUBLIC(ReadOnlyPart)

    ReadOnlyPartPrivate(ReadOnlyPart *q): PartPrivate(q)
    {
        m_job = nullptr;
        m_statJob = nullptr;
        m_uploadJob = nullptr;
        m_showProgressInfo = true;
        m_saveOk = false;
        m_waitForSave = false;
        m_duringSaveAs = false;
        m_bTemp = false;
        m_bAutoDetectedMime = false;
        m_closeUrlFromOpenUrl = false;
    }

    ~ReadOnlyPartPrivate()
    {
    }

    void _k_slotJobFinished(KJob *job);
    void _k_slotStatJobFinished(KJob *job);
    void _k_slotGotMimeType(KIO::Job *job, const QString &mime);
    bool openLocalFile();
    void openRemoteFile();

    KIO::FileCopyJob *m_job;
    KIO::StatJob *m_statJob;
    KIO::FileCopyJob *m_uploadJob;
    QUrl m_originalURL; // for saveAs
    QString m_originalFilePath; // for saveAs
    bool m_showProgressInfo : 1;
    bool m_saveOk : 1;
    bool m_waitForSave : 1;
    bool m_duringSaveAs : 1;

    /**
     * If @p true, @p m_file is a temporary file that needs to be deleted later.
     */
    bool m_bTemp: 1;

    // whether the mimetype in the arguments was detected by the part itself
    bool m_bAutoDetectedMime : 1;
    // Whether we are calling closeUrl() from openUrl().
    bool m_closeUrlFromOpenUrl;

    /**
     * Remote (or local) url - the one displayed to the user.
     */
    QUrl m_url;

    /**
     * Local file - the only one the part implementation should deal with.
     */
    QString m_file;

    OpenUrlArguments m_arguments;
};

} // namespace

#endif
