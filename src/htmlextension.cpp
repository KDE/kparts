/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2010 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "htmlextension.h"

#include "readonlypart.h"

using namespace KParts;

class KParts::HtmlExtensionPrivate
{
};

KParts::HtmlExtension::HtmlExtension(KParts::ReadOnlyPart *parent)
    : QObject(parent)
    , d(nullptr)
{
}

KParts::HtmlExtension::~HtmlExtension()
{
}

bool HtmlExtension::hasSelection() const
{
    return false;
}

HtmlExtension *KParts::HtmlExtension::childObject(QObject *obj)
{
    return obj->findChild<KParts::HtmlExtension *>(QString(), Qt::FindDirectChildrenOnly);
}
