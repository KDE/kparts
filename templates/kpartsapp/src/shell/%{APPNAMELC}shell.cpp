/*
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .
 */

#include "%{APPNAMELC}shell.h"

// KF headers
#include <KParts/ReadWritePart>
#include <KPluginLoader>
#include <KPluginFactory>
#include <KActionCollection>
#include <KStandardAction>
#include <KLocalizedString>
#include <KMessageBox>

// Qt headers
#include <QApplication>
#include <QFileDialog>

%{APPNAME}Shell::%{APPNAME}Shell()
    : KParts::MainWindow()
{
    // set KXMLUI resource file
    setXMLFile(QStringLiteral("%{APPNAMELC}ui.rc"));

    // setup our actions
    setupActions();

    // find and load the part
    // Doing it by name, which is a bad idea usually but alright here
    // since this part is made for this shell
    KPluginLoader loader(QStringLiteral("%{APPNAMELC}part"));
    auto factory = loader.factory();
    if (!factory) {
        // can't do anything useful without part, thus quit the app
        KMessageBox::error(this, i18n("Could not find %{APPNAME} part!"));

        qApp->quit();
        // return here, because qApp->quit() only means "exit the
        // next time we enter the event loop...
        return;
    }

    m_part = factory->create<KParts::ReadWritePart>(this);

    if (m_part) {
        // integrate and setup
        setCentralWidget(m_part->widget());
        setupGUI(ToolBar | Keys | StatusBar | Save);
        createGUI(m_part);
    }
}

%{APPNAME}Shell::~%{APPNAME}Shell()
{
}

void %{APPNAME}Shell::loadDocument(const QUrl& url)
{
    m_part->openUrl(url);
}

void %{APPNAME}Shell::setupActions()
{
    KStandardAction::openNew(this, &%{APPNAME}Shell::fileNew, actionCollection());
    KStandardAction::open(this, &%{APPNAME}Shell::fileOpen, actionCollection());

    KStandardAction::quit(qApp, &QApplication::closeAllWindows, actionCollection());
}

void %{APPNAME}Shell::fileNew()
{
    // open a new window if the document is _not_ in its initial state
    if (!m_part->url().isValid() || m_part->isModified()) {
        (new %{APPNAME}Shell)->show();
    };
}

void %{APPNAME}Shell::fileOpen()
{
    const QUrl url = QFileDialog::getOpenFileUrl(this);

    if (url.isValid()) {
        // open a new window if the document is _not_ in its initial state
        if (!m_part->url().isValid() || m_part->isModified()) {
            // open the file in a new window
            auto window = new %{APPNAME}Shell;
            window->loadDocument(url);
            window->show();
        } else {
            // open the file in this window
            loadDocument(url);
        }
    }
}
