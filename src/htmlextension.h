/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2010 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPARTS_HTMLEXTENSION_H
#define KPARTS_HTMLEXTENSION_H

#include <kparts/kparts_export.h>

#include <QObject>
#include <memory>

class QUrl;

namespace KParts
{
class ReadOnlyPart;
class HtmlExtensionPrivate;

/**
 * @class HtmlExtension htmlextension.h <KParts/HtmlExtension>
 *
 * @short An extension for KParts to provide HTML-related features
 *
 * Use qobject_cast to cast the extension to interesting interfaces, like
 * qobject_cast<KParts::SelectorInterface>.
 *
 * @since 4.6
 */
class KPARTS_EXPORT HtmlExtension : public QObject
{
    Q_OBJECT
public:
    explicit HtmlExtension(KParts::ReadOnlyPart *parent);
    ~HtmlExtension();

    /**
     * Queries @p obj for a child object which inherits from this
     * HtmlExtension class.
     */
    static HtmlExtension *childObject(QObject *obj);

    /**
     * Returns the current base url of the part that implements this extension.
     *
     * This function is mostly used to resolve any relative URLs that might be
     * returned when querying the part for links.
     */
    virtual QUrl baseUrl() const = 0;

    /**
     * Returns true if portions of the content in the part that implements
     * this extension are selected.
     *
     * By default this function returns false.
     */
    virtual bool hasSelection() const;

private:
    // for future extensions
    std::unique_ptr<HtmlExtensionPrivate> const d;
};

} // namespace KParts

#endif /* KPARTS_HTMLEXTENSION_H */
