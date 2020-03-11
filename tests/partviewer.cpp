/*
    Copyright (c) 2000 David Faure <faure@kde.org>
    Copyright (c) 2000 Simon Hausmann <hausmann@kde.org>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License or ( at
    your option ) version 3 or, at the discretion of KDE e.V. ( which shall
    act as a proxy as in section 14 of the GPLv3 ), any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "partviewer.h"

#include <kactioncollection.h>
#include <kactionmenu.h>
#include <klocalizedstring.h>
#include <kmimetypetrader.h>
#include <partloader.h>

#include <qmimedatabase.h>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QTest>

PartViewer::PartViewer()
{
    setXMLFile(QFINDTESTDATA("partviewer_shell.rc"));

    QAction *paOpen = new QAction(QIcon::fromTheme(QStringLiteral("document-open")), QStringLiteral("&Open file"), this);
    actionCollection()->addAction(QStringLiteral("file_open"), paOpen);
    connect(paOpen, SIGNAL(triggered()), this, SLOT(slotFileOpen()));

    QAction *paQuit = new QAction(QIcon::fromTheme(QStringLiteral("application-exit")), QStringLiteral("&Quit"), this);
    actionCollection()->addAction(QStringLiteral("file_quit"), paQuit);
    connect(paQuit, SIGNAL(triggered()), this, SLOT(close()));

    m_part = nullptr;

    // Set a reasonable size
    resize(600, 350);
}

PartViewer::~PartViewer()
{
    qDeleteAll(m_openWithActions);
    delete m_part;
}

void PartViewer::switchToPart(const QUrl &url)
{
    setCentralWidget(m_part->widget());
    // Integrate its GUI
    createGUI(m_part);

    m_part->openUrl(url);
}

void PartViewer::openUrl(const QUrl &url)
{
    unplugActionList(QStringLiteral("file_openwith"));
    delete m_part;
    QMimeDatabase db;
    const QString mimeType = db.mimeTypeForUrl(url).name();
    QString errorString;
    m_part = KParts::PartLoader::createPartInstanceForMimeType<KParts::ReadOnlyPart>(mimeType,
             this, this, &errorString);

    if (m_part) {
        switchToPart(url);
    } else {
        qWarning() << errorString;
    }

    // Show available parts in the GUI

    qDeleteAll(m_openWithActions);
    m_openWithActions.clear();
    const QVector<KPluginMetaData> plugins = KParts::PartLoader::partsForMimeType(mimeType);
    for (const KPluginMetaData &plugin : plugins) {
        QAction *action = new QAction(plugin.name(), this);
        connect(action, &QAction::triggered, this, [=] { loadPlugin(plugin, url); });
        m_openWithActions.append(action);
    }
    if (!m_openWithActions.isEmpty()) {
        QAction *sep = new QAction(this);
        sep->setSeparator(true);
        m_openWithActions.append(sep);
    }
    plugActionList(QStringLiteral("file_openwith"), m_openWithActions);
}

void PartViewer::slotFileOpen()
{
    QUrl url = QFileDialog::getOpenFileUrl();
    if (!url.isEmpty()) {
        openUrl(url);
    }
}

void PartViewer::loadPlugin(const KPluginMetaData &md, const QUrl &url)
{
    delete m_part;
    KPluginLoader loader(md.fileName());
    m_part = loader.factory()->create<KParts::ReadOnlyPart>(this, this);
    if (m_part) {
        switchToPart(url);
    }
}

int main(int argc, char **argv)
{
    // This is a test window for showing any part

    QApplication app(argc, argv);
    PartViewer *shell = new PartViewer;
    if (argc > 1) {
        QUrl url = QUrl::fromUserInput(QLatin1String(argv[1]));
        shell->openUrl(url);
    } else {
        shell->slotFileOpen();
    }
    shell->show();
    return app.exec();
}

