/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText:  1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "browserhostextension.h"

#include "readonlypart.h"

using namespace KParts;

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 78)

namespace KParts
{
class BrowserHostExtensionPrivate
{
public:
    BrowserHostExtensionPrivate()
    {
    }
    ~BrowserHostExtensionPrivate()
    {
    }

    KParts::ReadOnlyPart *m_part = nullptr;
};

}

BrowserHostExtension::BrowserHostExtension(KParts::ReadOnlyPart *parent)
    : QObject(parent)
    , d(new BrowserHostExtensionPrivate)
{
    d->m_part = parent;
}

BrowserHostExtension::~BrowserHostExtension() = default;

QStringList BrowserHostExtension::frameNames() const
{
    return QStringList();
}

const QList<KParts::ReadOnlyPart *> BrowserHostExtension::frames() const
{
    return QList<KParts::ReadOnlyPart *>();
}

bool BrowserHostExtension::openUrlInFrame(const QUrl &, const KParts::OpenUrlArguments &, const KParts::BrowserArguments &)
{
    return false;
}

BrowserHostExtension *BrowserHostExtension::childObject(QObject *obj)
{
    return obj->findChild<KParts::BrowserHostExtension *>(QString(), Qt::FindDirectChildrenOnly);
}

BrowserHostExtension *BrowserHostExtension::findFrameParent(KParts::ReadOnlyPart *callingPart, const QString &frame)
{
    Q_UNUSED(callingPart);
    Q_UNUSED(frame);
    return nullptr;
}

#endif
