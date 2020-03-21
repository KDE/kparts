/*
    Copyright (c) 2020 David Faure <faure@kde.org>

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

#include <KParts/ReadOnlyPart>
#include <KParts/PartLoader>
#include <QTest>

#include <KPluginMetaData>
#include <QDebug>
#include <QStandardPaths>

class PartLoaderTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
        cleanupTestCase();

        const QString desktopFile = QFINDTESTDATA(QStringLiteral("notepad.desktop"));
        QVERIFY(!desktopFile.isEmpty());
        const QString destDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/kservices5");
        QDir().mkpath(destDir);
        QVERIFY(QFile::copy(desktopFile, destDir + QLatin1String("/notepad.desktop")));
        // Ensure notepadpart is preferred over other installed parts.
        // This also tests the mimeapps.list parsing in PartLoader
        const QByteArray contents = "[Added KDE Service Associations]\n"
                                    "text/plain=notepad.desktop;\n";
        const QString configDir = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
        QDir().mkpath(configDir);
        const QString mimeAppsPath = configDir + QLatin1String("/mimeapps.list");
        QFile mimeAppsFile(mimeAppsPath);
        QVERIFY(mimeAppsFile.open(QIODevice::WriteOnly));
        mimeAppsFile.write(contents);
    }

    void cleanupTestCase()
    {
        const QString destDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/kservices5");
        QFile::remove(destDir + QLatin1String("/notepad.desktop"));
    }

    void shouldListParts()
    {
        // GIVEN
        const QString mimeType = QStringLiteral("text/plain");

        // WHEN
        const QVector<KPluginMetaData> plugins = KParts::PartLoader::partsForMimeType(mimeType);

        // THEN
        QVERIFY(!plugins.isEmpty());
//        for (const KPluginMetaData &metaData : plugins) {
//            qDebug() << metaData.fileName() << metaData.pluginId();
//        }
        QCOMPARE(plugins.at(0).pluginId(), QStringLiteral("notepadpart")); // basename of plugin
        const QString fileName = plugins.at(0).fileName();
        QVERIFY2(fileName.contains(QLatin1String("notepadpart")), qPrintable(fileName));
    }

    void shouldLoadPlainTextPart()
    {
        // GIVEN
        const QString mimeType = QStringLiteral("text/plain");
        const QString testFile = QFINDTESTDATA("partloadertest.cpp");
        QVERIFY(!testFile.isEmpty());
        QWidget parentWidget;
        QString errorString;

        // WHEN
        KParts::ReadOnlyPart *part = KParts::PartLoader::createPartInstanceForMimeType<KParts::ReadOnlyPart>(mimeType,
                 &parentWidget, this, &errorString);

        // THEN
        QVERIFY(part);
        QCOMPARE(errorString, QString());
        QCOMPARE(part->metaObject()->className(), "NotepadPart");
        QVERIFY(part->openUrl(QUrl::fromLocalFile(testFile)));
    }

    void shouldHandleNoPartError()
    {
        // GIVEN
        // can't use an unlikely mimetype here, okteta is associated with application/octet-stream :-)
        const QString mimeType = QStringLiteral("does/not/exist");
        QWidget parentWidget;
        QString errorString;

        // WHEN
        KParts::ReadOnlyPart *part = KParts::PartLoader::createPartInstanceForMimeType<KParts::ReadOnlyPart>(mimeType,
                 &parentWidget, this, &errorString);

        // THEN
        QVERIFY2(!part, part ? part->metaObject()->className() : nullptr);
        QCOMPARE(errorString, QStringLiteral("No part was found for mimeType does/not/exist"));
    }
};

QTEST_MAIN(PartLoaderTest)

#include "partloadertest.moc"
