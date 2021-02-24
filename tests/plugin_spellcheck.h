/*
    SPDX-FileCopyrightText: 2000 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef plugin_spellcheck_h
#define plugin_spellcheck_h

#include <kparts/plugin.h>

class PluginSpellCheck : public KParts::Plugin
{
    Q_OBJECT
public:
    PluginSpellCheck(QObject *parent = nullptr, const QVariantList & = QVariantList());
    virtual ~PluginSpellCheck();

public Q_SLOTS:
    void slotSpellCheck();
};

#endif
