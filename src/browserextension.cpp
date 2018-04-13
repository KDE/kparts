/* This file is part of the KDE project
  Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
            (C) 1999 David Faure <faure@kde.org>

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
#include "browserextension.h"

#include <klocalizedstring.h>
#include <kmessagebox.h>
#include <kurifilter.h>

#include <QApplication>
#include <QClipboard>
#include <QTimer>
#include <QMap>
#include <QMetaEnum>
#include <QRegExp>

#include <QDebug>

using namespace KParts;


namespace KParts
{

// Internal class, use to store the status of the actions
class KBitArray
{
public:
    int val;
    KBitArray()
    {
        val = 0;
    }
    bool operator [](int index)
    {
        return (val & (1 << index)) ? true : false;
    }
    void setBit(int index, bool value)
    {
        if (value) {
            val = val | (1 << index);
        } else {
            val = val & ~(1 << index);
        }
    }
};

class Q_DECL_HIDDEN BrowserExtension::BrowserExtensionPrivate
{
public:
    BrowserExtensionPrivate(KParts::ReadOnlyPart *parent)
        : m_urlDropHandlingEnabled(false),
          m_browserInterface(nullptr),
          m_part(parent)
    {}

    struct DelayedRequest {
        QUrl m_delayedURL;
        KParts::OpenUrlArguments m_delayedArgs;
        KParts::BrowserArguments m_delayedBrowserArgs;
    };

    QList<DelayedRequest> m_requests;
    bool m_urlDropHandlingEnabled;
    KBitArray m_actionStatus;
    QMap<int, QString> m_actionText;
    BrowserInterface *m_browserInterface;

    static void createActionSlotMap();

    KParts::ReadOnlyPart *m_part;
    OpenUrlArguments m_args;
    BrowserArguments m_browserArgs;
};

Q_GLOBAL_STATIC(BrowserExtension::ActionSlotMap, s_actionSlotMap)
Q_GLOBAL_STATIC(BrowserExtension::ActionNumberMap, s_actionNumberMap)

void BrowserExtension::BrowserExtensionPrivate::createActionSlotMap()
{
    s_actionSlotMap()->insert("cut", SLOT(cut()));
    s_actionSlotMap()->insert("copy", SLOT(copy()));
    s_actionSlotMap()->insert("paste", SLOT(paste()));
    s_actionSlotMap()->insert("print", SLOT(print()));
    // Tricky. Those aren't actions in fact, but simply methods that a browserextension
    // can have or not. No need to return them here.
    //s_actionSlotMap()->insert( "reparseConfiguration", SLOT(reparseConfiguration()) );
    //s_actionSlotMap()->insert( "refreshMimeTypes", SLOT(refreshMimeTypes()) );

    // Create the action-number map
    ActionSlotMap::ConstIterator it = s_actionSlotMap()->constBegin();
    ActionSlotMap::ConstIterator itEnd = s_actionSlotMap()->constEnd();
    for (int i = 0; it != itEnd; ++it, ++i) {
        //qDebug() << " action " << it.key() << " number " << i;
        s_actionNumberMap()->insert(it.key(), i);
    }
}

}

BrowserExtension::BrowserExtension(KParts::ReadOnlyPart *parent)
    : QObject(parent), d(new BrowserExtensionPrivate(parent))
{
    //qDebug() << "BrowserExtension::BrowserExtension() " << this;

    if (s_actionSlotMap()->isEmpty())
        // Create the action-slot map
    {
        BrowserExtensionPrivate::createActionSlotMap();
    }

    // Set the initial status of the actions depending on whether
    // they're supported or not
    const QMetaObject *metaobj = metaObject();
    ActionSlotMap::ConstIterator it = s_actionSlotMap()->constBegin();
    ActionSlotMap::ConstIterator itEnd = s_actionSlotMap()->constEnd();
    for (int i = 0; it != itEnd; ++it, ++i) {
        // Does the extension have a slot with the name of this action ?
        QByteArray slotSig = it.key() + "()";
        d->m_actionStatus.setBit(i, metaobj->indexOfMethod(slotSig.constData()) != -1);
    }

    connect(d->m_part, static_cast<void (KParts::ReadOnlyPart::*)()>(&KParts::ReadOnlyPart::completed),
            this, &BrowserExtension::slotCompleted);
    connect(this, &BrowserExtension::openUrlRequest,
            this, &BrowserExtension::slotOpenUrlRequest);
    connect(this, &BrowserExtension::enableAction,
            this, &BrowserExtension::slotEnableAction);
    connect(this, &BrowserExtension::setActionText,
            this, &BrowserExtension::slotSetActionText);
}

BrowserExtension::~BrowserExtension()
{
    //qDebug() << "BrowserExtension::~BrowserExtension() " << this;
    delete d;
}

void BrowserExtension::setBrowserArguments(const BrowserArguments &args)
{
    d->m_browserArgs = args;
}

BrowserArguments BrowserExtension::browserArguments() const
{
    return d->m_browserArgs;
}

int BrowserExtension::xOffset()
{
    return 0;
}

int BrowserExtension::yOffset()
{
    return 0;
}

void BrowserExtension::saveState(QDataStream &stream)
{
    // TODO add d->m_part->mimeType()
    stream << d->m_part->url() << static_cast<qint32>(xOffset()) << static_cast<qint32>(yOffset());
}

void BrowserExtension::restoreState(QDataStream &stream)
{
    QUrl u;
    qint32 xOfs, yOfs;
    stream >> u >> xOfs >> yOfs;

    OpenUrlArguments args;
    args.setXOffset(xOfs);
    args.setYOffset(yOfs);
    // TODO add args.setMimeType
    d->m_part->setArguments(args);
    d->m_part->openUrl(u);
}

bool BrowserExtension::isURLDropHandlingEnabled() const
{
    return d->m_urlDropHandlingEnabled;
}

void BrowserExtension::setURLDropHandlingEnabled(bool enable)
{
    d->m_urlDropHandlingEnabled = enable;
}

void BrowserExtension::slotCompleted()
{
    //empty the argument stuff, to avoid bogus/invalid values when opening a new url
    setBrowserArguments(BrowserArguments());
}

void BrowserExtension::pasteRequest()
{
    QString plain(QStringLiteral("plain"));
    QString url = QApplication::clipboard()->text(plain, QClipboard::Selection).trimmed();
    // Remove linefeeds and any whitespace surrounding it.
    url.remove(QRegExp(QStringLiteral("[\\ ]*\\n+[\\ ]*")));

    // Check if it's a URL
    QStringList filters = KUriFilter::self()->pluginNames();
    filters.removeAll(QStringLiteral("kuriikwsfilter"));
    filters.removeAll(QStringLiteral("localdomainurifilter"));
    KUriFilterData filterData;
    filterData.setData(url);
    filterData.setCheckForExecutables(false);
    if (KUriFilter::self()->filterUri(filterData, filters)) {
        switch (filterData.uriType()) {
        case KUriFilterData::LocalFile:
        case KUriFilterData::LocalDir:
        case KUriFilterData::NetProtocol:
            slotOpenUrlRequest(filterData.uri());
            break;
        case KUriFilterData::Error:
            KMessageBox::sorry(d->m_part->widget(), filterData.errorMsg());
            break;
        default:
            break;
        }
    } else if (KUriFilter::self()->filterUri(filterData, QStringList(QStringLiteral("kuriikwsfilter"))) &&
               url.length() < 250) {
        if (KMessageBox::questionYesNo(d->m_part->widget(),
                                       i18n("<qt>Do you want to search the Internet for <b>%1</b>?</qt>", url.toHtmlEscaped()),
                                       i18n("Internet Search"), KGuiItem(i18n("&Search"), QStringLiteral("edit-find")),
                                       KStandardGuiItem::cancel(), QStringLiteral("MiddleClickSearch")) == KMessageBox::Yes) {
            slotOpenUrlRequest(filterData.uri());
        }
    }
}

void BrowserExtension::slotOpenUrlRequest(const QUrl &url, const KParts::OpenUrlArguments &args, const KParts::BrowserArguments &browserArgs)
{
    //qDebug() << this << " BrowserExtension::slotOpenURLRequest(): url=" << url.url();
    BrowserExtensionPrivate::DelayedRequest req;
    req.m_delayedURL = url;
    req.m_delayedArgs = args;
    req.m_delayedBrowserArgs = browserArgs;
    d->m_requests.append(req);
    QTimer::singleShot(0, this, SLOT(slotEmitOpenUrlRequestDelayed()));
}

void BrowserExtension::slotEmitOpenUrlRequestDelayed()
{
    if (d->m_requests.isEmpty()) {
        return;
    }
    BrowserExtensionPrivate::DelayedRequest req = d->m_requests.front();
    d->m_requests.pop_front();
    emit openUrlRequestDelayed(req.m_delayedURL, req.m_delayedArgs, req.m_delayedBrowserArgs);
    // tricky: do not do anything here! (no access to member variables, etc.)
}

void BrowserExtension::setBrowserInterface(BrowserInterface *impl)
{
    d->m_browserInterface = impl;
}

BrowserInterface *BrowserExtension::browserInterface() const
{
    return d->m_browserInterface;
}

void BrowserExtension::slotEnableAction(const char *name, bool enabled)
{
    //qDebug() << "BrowserExtension::slotEnableAction " << name << " " << enabled;
    ActionNumberMap::ConstIterator it = s_actionNumberMap()->constFind(name);
    if (it != s_actionNumberMap()->constEnd()) {
        d->m_actionStatus.setBit(it.value(), enabled);
        //qDebug() << "BrowserExtension::slotEnableAction setting bit " << it.data() << " to " << enabled;
    } else {
        qWarning() << "BrowserExtension::slotEnableAction unknown action " << name;
    }
}

bool BrowserExtension::isActionEnabled(const char *name) const
{
    int actionNumber = (*s_actionNumberMap())[ name ];
    return d->m_actionStatus[ actionNumber ];
}

void BrowserExtension::slotSetActionText(const char *name, const QString &text)
{
    //qDebug() << "BrowserExtension::slotSetActionText " << name << " " << text;
    ActionNumberMap::ConstIterator it = s_actionNumberMap()->constFind(name);
    if (it != s_actionNumberMap()->constEnd()) {
        d->m_actionText[ it.value() ] = text;
    } else {
        qWarning() << "BrowserExtension::slotSetActionText unknown action " << name;
    }
}

QString BrowserExtension::actionText(const char *name) const
{
    int actionNumber = (*s_actionNumberMap())[ name ];
    QMap<int, QString>::ConstIterator it = d->m_actionText.constFind(actionNumber);
    if (it != d->m_actionText.constEnd()) {
        return *it;
    }
    return QString();
}

// for compatibility
BrowserExtension::ActionSlotMap BrowserExtension::actionSlotMap()
{
    return *actionSlotMapPtr();
}

BrowserExtension::ActionSlotMap *BrowserExtension::actionSlotMapPtr()
{
    if (s_actionSlotMap()->isEmpty()) {
        BrowserExtensionPrivate::createActionSlotMap();
    }
    return s_actionSlotMap();
}

BrowserExtension *BrowserExtension::childObject(QObject *obj)
{
    return obj->findChild<KParts::BrowserExtension *>(QString(), Qt::FindDirectChildrenOnly);
}
