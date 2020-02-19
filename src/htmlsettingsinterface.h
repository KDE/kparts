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

#ifndef KPARTS_HTMLSETTINGSINTERFACE_H
#define KPARTS_HTMLSETTINGSINTERFACE_H

#include <kparts/kparts_export.h>

#include <QObject>

class QString;


namespace KParts
{

/**
 * @class HtmlSettingsInterface htmlsettingsinterface.h <KParts/HtmlSettingsInterface>
 *
 * @short An interface for modifying the settings of browser engines.
 *
 *  This interface provides a generic means for querying or changing the
 *  settings of browser engines that implement it.
 *
 *  To use this class simply cast an instance of the HTMLExtension object
 *  using qobject_cast<KParts::HtmlSettingsInterface>.
 *
 *  Example:
 *  @code
 *  KParts::HTMLExtension* extension = KParts::HTMLExtension::childObject(part);
 *  KParts::HtmlSettingsInterface* settings = qobject_cast&lt;KParts::HtmlSettingsInterface&gt;(extension);
 *  const bool autoLoadImages = settings->attribute(KParts::AutoLoadImages);
 *  @endcode
 *
 *  @since 4.8.1
 */
class KPARTS_EXPORT HtmlSettingsInterface
{
public:
    /**
     * Settings attribute types.
     */
    enum HtmlSettingsType {
        AutoLoadImages,
        DnsPrefetchEnabled,
        JavaEnabled,
        JavascriptEnabled,
        MetaRefreshEnabled,
        PluginsEnabled,
        PrivateBrowsingEnabled,
        OfflineStorageDatabaseEnabled,
        OfflineWebApplicationCacheEnabled,
        LocalStorageEnabled,
        UserDefinedStyleSheetURL
    };

    /**
     * This enum specifies whether Java/JavaScript execution is allowed.
     *
     * @since 4.8.2
     */
    enum JavaScriptAdvice {
        JavaScriptDunno = 0,
        JavaScriptAccept,
        JavaScriptReject
    };

    /**
     * This enum specifies the policy for window.open
     *
     * @since 4.8.2
     */
    enum JSWindowOpenPolicy {
        JSWindowOpenAllow = 0,
        JSWindowOpenAsk,
        JSWindowOpenDeny,
        JSWindowOpenSmart
    };

    /**
     * This enum specifies the policy for window.status and .defaultStatus
     *
     * @since 4.8.2
     */
    enum JSWindowStatusPolicy {
        JSWindowStatusAllow = 0,
        JSWindowStatusIgnore
    };

    /**
     * This enum specifies the policy for window.moveBy and .moveTo
     *
     * @since 4.8.2
     */
    enum JSWindowMovePolicy {
        JSWindowMoveAllow = 0,
        JSWindowMoveIgnore
    };

    /**
     * This enum specifies the policy for window.resizeBy and .resizeTo
     *
     * @since 4.8.2
     */
    enum JSWindowResizePolicy {
        JSWindowResizeAllow = 0,
        JSWindowResizeIgnore
    };

    /**
     * This enum specifies the policy for window.focus
     *
     * @since 4.8.2
     */
    enum JSWindowFocusPolicy {
        JSWindowFocusAllow = 0,
        JSWindowFocusIgnore
    };

    /**
     * Destructor
     */
    virtual ~HtmlSettingsInterface() {}

    /**
     * Returns the value of the browser engine's attribute @p type.
     */
    virtual QVariant htmlSettingsProperty(HtmlSettingsType type) const = 0;

    /**
     * Sets the value of the browser engine's attribute @p type to @p value.
     */
    virtual bool setHtmlSettingsProperty(HtmlSettingsType type, const QVariant &value) = 0;

    /**
     * A convenience function that returns the javascript advice for @p text.
     *
     * If text is not either "accept" or "reject", this function returns
     * @ref JavaScriptDunno.
     *
     *  @since 4.8.2
     */
    static JavaScriptAdvice textToJavascriptAdvice(const QString &text);

    /**
     * A convenience function Returns the text for the given JavascriptAdvice @p advice.
     *
     * If @p advice is not either JavaScriptAccept or JavaScriptReject, this
     * function returns a NULL string.
     *
     *  @since 4.8.2
     */
    static const char *javascriptAdviceToText(JavaScriptAdvice advice);

    /**
     * A convenience function that splits @p text into @p domain, @p javaAdvice
     * and @p jScriptAdvice.
     *
     * If @p text is empty or does not contain the proper delimiter (':'), this
     * function will set @p domain to @p text and the other two parameters to
     * JavaScriptDunno.
     *
     *  @since 4.8.2
     */
    static void splitDomainAdvice(const QString &text,
                                  QString &domain,
                                  JavaScriptAdvice &javaAdvice,
                                  JavaScriptAdvice &javaScriptAdvice);
};

} // namespace KParts

Q_DECLARE_INTERFACE(KParts::HtmlSettingsInterface,
                    "org.kde.KParts.HtmlSettingsInterface")

#endif /* KPARTS_HTMLSETTINGSINTERFACE_H */
