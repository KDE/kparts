/*
    SPDX-FileCopyrightText: 1999, 2000 David Faure <faure@kde.org>
    SPDX-FileCopyrightText: 1999, 2000 Simon Hausmann <hausmann@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "notepad.h"
#include <kparts/mainwindow.h>
#include <kparts/partmanager.h>

#include <QAction>
#include <QFile>
#include <QTextEdit>
#include <QTextStream>

#include <KAboutData>
#include <KActionCollection>
#include <KLocalizedString>
#include <QDebug>

K_PLUGIN_FACTORY_WITH_JSON(NotepadFactory, "notepad.json", registerPlugin<NotepadPart>();)

NotepadPart::NotepadPart(QWidget *parentWidget, QObject *parent, const KPluginMetaData &metaData, const QVariantList &)
    : KParts::ReadWritePart(parent)
{
    setMetaData(metaData);

    m_edit = new QTextEdit(parentWidget);
    m_edit->setPlainText(QStringLiteral("NotepadPart's multiline edit"));
    setWidget(m_edit);

    QAction *searchReplace = new QAction(QStringLiteral("Search and replace"), this);
    actionCollection()->addAction(QStringLiteral("searchreplace"), searchReplace);
    connect(searchReplace, &QAction::triggered, this, &NotepadPart::slotSearchReplace);

    setXMLFile(QStringLiteral("notepadpart.rc")); // will be found in the qrc resource

    setReadWrite(true);

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 90)
    // Always write this as the last line of your constructor
    loadPlugins();
#endif
}

NotepadPart::~NotepadPart()
{
}

void NotepadPart::setReadWrite(bool rw)
{
    m_edit->setReadOnly(!rw);
    if (rw) {
        connect(m_edit, &QTextEdit::textChanged, this, qOverload<>(&KParts::ReadWritePart::setModified));
    } else {
        disconnect(m_edit, &QTextEdit::textChanged, this, qOverload<>(&KParts::ReadWritePart::setModified));
    }

    ReadWritePart::setReadWrite(rw);
}

bool NotepadPart::openFile()
{
    // qDebug() << "NotepadPart: opening " << localFilePath();
    QFile f(localFilePath());
    QString s;
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream t(&f);
        // The default with Qt6 is UTF-8
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        t.setCodec("UTF-8");
#endif
        s = t.readAll();
        f.close();
    }
    m_edit->setPlainText(s);

    Q_EMIT setStatusBarText(url().toString());

    return true;
}

bool NotepadPart::saveFile()
{
    if (!isReadWrite()) {
        return false;
    }
    QFile f(localFilePath());
    if (f.open(QIODevice::WriteOnly)) {
        QTextStream t(&f);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        t.setCodec("UTF-8");
#endif
        t << m_edit->toPlainText();
        f.close();
        return true;
    } else {
        return false;
    }
}

void NotepadPart::slotSearchReplace()
{
}

#include "notepad.moc"
