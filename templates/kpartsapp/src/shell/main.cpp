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
#include <KAboutData>
#include <KLocalizedString>

// Qt headers
#include <QApplication>
#include <QCommandLineParser>
#include <QUrl>
#include <QDir>
#include <QIcon>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("%{APPNAMELC}");

    KAboutData aboutData(QStringLiteral("%{APPNAMELC}"),
        i18n("%{APPNAME}"),
        QStringLiteral("%{VERSION}"),
        i18n("A KPart Application"),
        KAboutLicense::GPL,
        i18n("Copyright %{CURRENT_YEAR} %{AUTHOR}"));
    aboutData.addAuthor(i18n("%{AUTHOR}"), i18n("Author"), QStringLiteral("%{EMAIL}"));
    aboutData.setOrganizationDomain("example.org");
    aboutData.setDesktopFileName(QStringLiteral("org.example.%{APPNAMELC}"));

    KAboutData::setApplicationData(aboutData);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("%{APPNAMELC}")));

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.addPositionalArgument(QStringLiteral("urls"), i18n("Document(s) to load."), QStringLiteral("[urls...]"));

    parser.process(app);
    aboutData.processCommandLine(&parser);

    const auto urls = parser.positionalArguments();

    if (urls.isEmpty()) {
        auto window = new %{APPNAME}Shell;
        window->show();
    } else {
        for (const auto &url : urls) {
            auto window = new %{APPNAME}Shell;
            window->show();
            window->loadDocument(QUrl::fromUserInput(url, QDir::currentPath(), QUrl::AssumeLocalFile));
        }
    }

    return app.exec();
}
