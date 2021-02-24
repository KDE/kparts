/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2020 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPARTS_METADATAUTIL_H
#define KPARTS_METADATAUTIL_H

#include <kparts/kparts_export.h>

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)

class KPluginMetaData;
class KAboutData;

namespace PartMetaDataUtil
{
KPluginMetaData fromKAboutData(const KAboutData &aboutData);

}

#endif

#endif
