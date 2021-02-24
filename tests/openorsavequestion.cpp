/*
    SPDX-FileCopyrightText: 2009 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include <QApplication>
#include <QDebug>
#include <browseropenorsavequestion.h>

using namespace KParts;

int main(int argc, char **argv)
{
    QApplication::setApplicationName(QStringLiteral("openorsavequestion"));
    QApplication app(argc, argv);

    // A test for both 1) "unknown mimetype"  2) no associated app
    {
        BrowserOpenOrSaveQuestion questionOpenUnknownMimeType(nullptr,
                                                              QUrl(QStringLiteral("http://www.example.com/foo.foo")),
                                                              QString::fromLatin1("application/foo"));
        BrowserOpenOrSaveQuestion::Result res = questionOpenUnknownMimeType.askOpenOrSave();
        qDebug() << res;
    }

    // The normal case
    {
        BrowserOpenOrSaveQuestion questionOpen(nullptr, QUrl(QStringLiteral("http://www.example.com/foo.pdf")), QString::fromLatin1("application/pdf"));
        questionOpen.setSuggestedFileName(QString::fromLatin1("file.pdf"));
        questionOpen.setFeatures(BrowserOpenOrSaveQuestion::ServiceSelection);
        BrowserOpenOrSaveQuestion::Result res = questionOpen.askOpenOrSave();
        qDebug() << res;
        if (res == BrowserOpenOrSaveQuestion::Open && questionOpen.selectedService()) {
            qDebug() << "Selected service:" << questionOpen.selectedService()->entryPath();
        }
    }

    // Trying a case with only one app associated
    {
        BrowserOpenOrSaveQuestion questionOpen(nullptr, QUrl(QStringLiteral("http://www.example.com/foo.zip")), QString::fromLatin1("application/zip"));
        questionOpen.setFeatures(BrowserOpenOrSaveQuestion::ServiceSelection);
        BrowserOpenOrSaveQuestion::Result res = questionOpen.askOpenOrSave();
        qDebug() << res;
        if (res == BrowserOpenOrSaveQuestion::Open && questionOpen.selectedService()) {
            qDebug() << "Selected service:" << questionOpen.selectedService()->entryPath();
        }
    }

    return 0;
}
