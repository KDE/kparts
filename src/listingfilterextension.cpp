/* This file is part of the KDE project
   Copyright (C) 2012 Dawit Alemayehu <adawit@kde.org>

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

#include "listingfilterextension.h"

#include "readonlypart.h"

KParts::ListingFilterExtension::ListingFilterExtension(KParts::ReadOnlyPart *parent)
    : QObject(parent),
      d(0)
{
}

KParts::ListingFilterExtension::~ListingFilterExtension()
{
}

KParts::ListingFilterExtension *KParts::ListingFilterExtension::childObject(QObject *obj)
{
    return obj->findChild<KParts::ListingFilterExtension *>(QString(), Qt::FindDirectChildrenOnly);
}

KParts::ListingFilterExtension::FilterModes KParts::ListingFilterExtension::supportedFilterModes() const
{
    return None;
}

bool KParts::ListingFilterExtension::supportsMultipleFilters(KParts::ListingFilterExtension::FilterMode) const
{
    return false;
}
