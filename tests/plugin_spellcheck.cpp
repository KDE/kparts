/*
    SPDX-FileCopyrightText: 2000 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "plugin_spellcheck.h"

#include <KActionCollection>
#include <KLocalizedString>
#include <KMessageBox>
#include <KParts/ReadOnlyPart>
#include <KPluginFactory>
#include <QAction>
#include <QDebug>
#include <QTextEdit>

PluginSpellCheck::PluginSpellCheck(QObject *parent, const QVariantList &)
    : Plugin(parent)
{
    QAction *act = new QAction(QStringLiteral("&Select current line (plugin)"), this);
    actionCollection()->addAction(QStringLiteral("spellcheck"), act);
    connect(act, &QAction::triggered, this, &PluginSpellCheck::slotSpellCheck);
}

PluginSpellCheck::~PluginSpellCheck()
{
}

void PluginSpellCheck::slotSpellCheck()
{
    // qDebug() << "Plugin parent : " << parent()->objectName() << " (" << parent()->metaObject()->className() << ")";
    // The parent is assumed to be a NotepadPart
    // Can't use qobject_cast here, we would need NotepadPart to be in a shared library.
    if (!parent()->inherits("NotepadPart")) {
        KMessageBox::error(nullptr, QStringLiteral("You just called the spell-check action on a wrong part (not NotepadPart)"));
    } else {
        KParts::ReadOnlyPart *part = static_cast<KParts::ReadOnlyPart *>(parent());
        QTextEdit *widget = qobject_cast<QTextEdit *>(part->widget());
        Q_ASSERT(widget);
        widget->selectAll();
    }
}

K_PLUGIN_FACTORY(PluginSpellCheckFactory, registerPlugin<PluginSpellCheck>();)

#include "moc_plugin_spellcheck.cpp"
#include "plugin_spellcheck.moc"
