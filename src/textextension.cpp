/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2010 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textextension.h"

#include "readonlypart.h"

class KParts::TextExtensionPrivate
{
};

KParts::TextExtension::TextExtension(KParts::ReadOnlyPart *parent)
    : QObject(parent)
    , d(nullptr)
{
}

KParts::TextExtension::~TextExtension()
{
}

bool KParts::TextExtension::hasSelection() const
{
    return false;
}

QString KParts::TextExtension::selectedText(Format) const
{
    return QString();
}

QString KParts::TextExtension::completeText(Format) const
{
    return QString();
}

KParts::TextExtension *KParts::TextExtension::childObject(QObject *obj)
{
    return obj->findChild<KParts::TextExtension *>(QString(), Qt::FindDirectChildrenOnly);
}

int KParts::TextExtension::pageCount() const
{
    return 0;
}

int KParts::TextExtension::currentPage() const
{
    return 0;
}

QString KParts::TextExtension::pageText(Format) const
{
    return QString();
}

bool KParts::TextExtension::findText(const QString &, KFind::SearchOptions) const
{
    return false;
}
