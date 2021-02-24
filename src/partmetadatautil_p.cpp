/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2020 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "partmetadatautil_p.h"

// KF
#include <KAboutData>
#include <KPluginMetaData>
// Qt
#include <QJsonArray>
#include <QJsonObject>

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)

namespace PartMetaDataUtil
{
static QJsonArray jsonArrayFromKAboutPersonList(const QList<KAboutPerson> &persons)
{
    QJsonArray array;
    for (const auto &person : persons) {
        QJsonObject authorsObject;
        authorsObject[QStringLiteral("Name")] = person.name();
        authorsObject[QStringLiteral("Email")] = person.emailAddress();
        authorsObject[QStringLiteral("Website")] = person.webAddress();
        authorsObject[QStringLiteral("Task")] = person.task();
        authorsObject[QStringLiteral("UserName")] = person.ocsUsername();
        array.append(authorsObject);
    }
    return array;
}

KPluginMetaData fromKAboutData(const KAboutData &aboutData)
{
    QJsonObject kplugin;
    kplugin[QStringLiteral("Id")] = aboutData.componentName();
    kplugin[QStringLiteral("Name")] = aboutData.displayName();
    kplugin[QStringLiteral("Description")] = aboutData.shortDescription();
    kplugin[QStringLiteral("Version")] = aboutData.version();
    const auto licenses = aboutData.licenses();
    if (!licenses.isEmpty()) {
        kplugin[QStringLiteral("License")] = licenses.first().spdx();
    }
    kplugin[QStringLiteral("Copyright")] = aboutData.copyrightStatement();
    kplugin[QStringLiteral("ExtraInformation")] = aboutData.otherText();
    kplugin[QStringLiteral("Website")] = aboutData.homepage();
#if KCOREADDONS_BUILD_DEPRECATED_SINCE(5, 2)
    QT_WARNING_PUSH
    QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
    QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
    kplugin[QStringLiteral("Icon")] = aboutData.programIconName();
    QT_WARNING_POP
#endif

    kplugin[QStringLiteral("Authors")] = jsonArrayFromKAboutPersonList(aboutData.authors());
    kplugin[QStringLiteral("Translators")] = jsonArrayFromKAboutPersonList(aboutData.translators());
    kplugin[QStringLiteral("OtherContributors")] = jsonArrayFromKAboutPersonList(aboutData.credits());

    QJsonObject json;
    json[QStringLiteral("KPlugin")] = kplugin;

    KPluginMetaData metaData(json, QString());
    return metaData;
}

}

#endif
