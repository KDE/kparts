/*
    Copyright (c) 2007 David Faure <faure@kde.org>

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

#ifndef PARTTEST_H
#define PARTTEST_H

#include <QtCore/QObject>

class PartTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testAutoDeletePart();
    void testAutoDeleteWidget();
    void testNoAutoDeletePart();
    void testNoAutoDeleteWidget();

    void testOpenUrlArguments();
    void testAutomaticMimeType();
    void testEmptyUrlAfterCloseUrl();

    void testToolbarVisibility();
};

#endif /* PARTTEST_H */
