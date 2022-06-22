/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2010 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectorinterface.h"

#include <QHash>

using namespace KParts;

SelectorInterface::QueryMethods SelectorInterface::supportedQueryMethods() const
{
    return KParts::SelectorInterface::None;
}

class Q_DECL_HIDDEN SelectorInterface::ElementPrivate : public QSharedData
{
public:
    QString tag;
    QHash<QString, QString> attributes;
};

SelectorInterface::Element::Element()
    : d(new ElementPrivate)
{
}

SelectorInterface::Element::Element(const SelectorInterface::Element &other)
    : d(other.d)
{
}

SelectorInterface::Element::~Element()
{
}

bool SelectorInterface::Element::isNull() const
{
    return d->tag.isNull();
}

void SelectorInterface::Element::setTagName(const QString &tag)
{
    d->tag = tag;
}

QString SelectorInterface::Element::tagName() const
{
    return d->tag;
}

void SelectorInterface::Element::setAttribute(const QString &name, const QString &value)
{
    d->attributes[name] = value; // insert or replace
}

QStringList SelectorInterface::Element::attributeNames() const
{
    return d->attributes.keys();
}

QString SelectorInterface::Element::attribute(const QString &name, const QString &defaultValue) const
{
    return d->attributes.value(name, defaultValue);
}

bool SelectorInterface::Element::hasAttribute(const QString &name) const
{
    return d->attributes.contains(name);
}
