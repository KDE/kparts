/* This file is part of the KDE project
   Copyright (C) 2010 David Faure <faure@kde.org>

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

#ifndef KPARTS_SELECTORINTERFACE_H
#define KPARTS_SELECTORINTERFACE_H

#include <kparts/kparts_export.h>

#include <QSharedDataPointer>
#include <QObject>

class QString;
class QStringList;
template <typename T> class QList;

namespace KParts
{

/**
 * @class SelectorInterface selectorinterface.h <KParts/SelectorInterface>
 *
 * @short Optional base class for HtmlExtension-derived classes
 * Provides DOM Selector like API: querySelector and querySelectorAll,
 * in order to find specific elements in an HTML document.
 *
 * Example:
 * @code
 * const QList<SelectorInterface::Element> elements = selectorInterface->querySelectorAll("head > link[rel=\"alternate\"]");
 * @endcode
 */
class KPARTS_EXPORT SelectorInterface
{
public:
    class ElementPrivate;
    class Element;

    /**
     * Query methods.
     * @see QueryMethods
     */
    enum QueryMethod {
        None = 0x00,                   /*!< Quering is not possible. */
        EntireContent = 0x01,          /*!< Query or can query the entire content. */
        SelectedContent = 0x02         /*!< Query or can query only the user selected content, if any. */
    };
    /**
     * Stores a combination of #QueryMethod values.
     */
    Q_DECLARE_FLAGS(QueryMethods, QueryMethod)

    /**
     * Destructor
     */
    virtual ~SelectorInterface() {}

    /**
     * Returns the supported query methods.
     *
     * By default this function returns None.
     *
     * @see QueryMethod
     */
    virtual QueryMethods supportedQueryMethods() const;

    /**
     * Returns the first (in document order) element in this fragment matching
     * the given CSS selector @p query and querying method @p method.
     *
     * Note that since the returned item is static snapshot, i.e. not live, it
     * will not be updated when the document changes.
     *
     * If the quering method specified by @p method is not supported or cannot be
     * handled, then a null element is returned.
     *
     * @see supportedQueryMethods
     * @see QueryMethod
     */
    virtual Element querySelector(const QString &query, QueryMethod method) const = 0;

    /**
     * Returns all (in document order) elements in this fragment matching the
     * given CSS selector @p query and querying method @p method.
     *
     * Note that since the returned list is static snapshot, i.e. not live, it
     * will not be updated when the document changes.
     *
     * If the quering method specified by @p method is not supported or cannot be
     * handled, then an empty list is returned.
     *
     * @see supportedQueryMethods
     * @see QueryMethod
     */
    virtual QList<Element> querySelectorAll(const QString &query, QueryMethod method) const = 0;

    class KPARTS_EXPORT Element
    {
    public:
        /**
         * Constructor
         */
        Element();

        /**
         * Copy constructor
         */
        Element(const Element &other);

        /**
         * Destructor
         */
        ~Element();

        /**
         * Returns true if the element is null ; otherwise returns false.
         */
        bool isNull() const;

        /**
         * Sets the tag name of this element.
         */
        void setTagName(const QString &tag);

        /**
         * Returns the tag name of this element.
         */
        QString tagName() const;

        /**
         * Adds an attribute with the given name and value.
         * If an attribute with the same name exists, its value is replaced by value.
         */
        void setAttribute(const QString &name, const QString &value);

        /**
         * Returns the list of attributes in this element.
         */
        QStringList attributeNames() const;

        /**
         * Returns the attribute with the given name. If the attribute does not exist, defaultValue is returned.
         */
        QString attribute(const QString &name, const QString &defaultValue = QString()) const;

        /**
         * Returns true if the attribute with the given @p name exists.
         */
        bool hasAttribute(const QString &name) const;

        // No namespace support yet, could be added with attributeNS, setAttributeNS

        /**
         * Swaps the contents of @p other with the contents of this.
         */
        void swap(Element &other)
        {
            d.swap(other.d);
        }

        /**
         * Assignment operator
         */
        Element &operator=(const Element &other)
        {
            if (this != &other) {
                Element copy(other);
                swap(copy);
            }
            return *this;
        }

    private:
        QSharedDataPointer<ElementPrivate> d;
    };
};

} // namespace KParts

inline void qSwap(KParts::SelectorInterface::Element &lhs, KParts::SelectorInterface::Element &rhs)
{
    lhs.swap(rhs);
}

Q_DECLARE_OPERATORS_FOR_FLAGS(KParts::SelectorInterface::QueryMethods)

Q_DECLARE_TYPEINFO(KParts::SelectorInterface::Element, Q_MOVABLE_TYPE);

Q_DECLARE_INTERFACE(KParts::SelectorInterface,
                    "org.kde.KParts.SelectorInterface")

#endif /* KPARTS_SELECTORINTERFACE_H */
