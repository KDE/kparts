/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999-2005 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "part.h"
#include "part_p.h"

#include "kparts_logging.h"
#include "partactivateevent.h"

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 72)
#include "partselectevent.h"
#endif

#include "guiactivateevent.h"
#include "partmanager.h"

#include <KIconLoader>
#include <KXMLGUIFactory>

using namespace KParts;

Part::Part(QObject *parent)
    : QObject(parent)
    , PartBase(*new PartPrivate(this))
{
    PartBase::setPartObject(this);
}

Part::Part(PartPrivate &dd, QObject *parent)
    : QObject(parent)
    , PartBase(dd)
{
    PartBase::setPartObject(this);
}

Part::~Part()
{
    Q_D(Part);

    // qCDebug(KPARTSLOG) << this;

    if (d->m_widget) {
        // We need to disconnect first, to avoid calling it !
        disconnect(d->m_widget.data(), &QWidget::destroyed, this, &Part::slotWidgetDestroyed);
    }

    if (d->m_manager) {
        d->m_manager->removePart(this);
    }

    if (d->m_widget && d->m_autoDeleteWidget) {
        // qCDebug(KPARTSLOG) << "deleting widget" << d->m_widget << d->m_widget->objectName();
        delete static_cast<QWidget *>(d->m_widget);
    }

    delete d->m_iconLoader;
}

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
void Part::embed(QWidget *parentWidget)
{
    if (widget()) {
        widget()->setParent(parentWidget, Qt::WindowFlags());
        widget()->setGeometry(0, 0, widget()->width(), widget()->height());
        widget()->show();
    }
}
#endif

QWidget *Part::widget()
{
    Q_D(Part);

    return d->m_widget;
}

void Part::setAutoDeleteWidget(bool autoDeleteWidget)
{
    Q_D(Part);
    d->m_autoDeleteWidget = autoDeleteWidget;
}

void Part::setAutoDeletePart(bool autoDeletePart)
{
    Q_D(Part);
    d->m_autoDeletePart = autoDeletePart;
}

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 82)
KIconLoader *Part::iconLoader()
{
    Q_D(Part);

    if (!d->m_iconLoader) {
        d->m_iconLoader = new KIconLoader(componentName());
    }
    return d->m_iconLoader;
}
#endif

KPluginMetaData Part::metaData() const
{
    Q_D(const Part);

    return d->m_metaData;
}

void Part::setManager(PartManager *manager)
{
    Q_D(Part);

    d->m_manager = manager;
}

PartManager *Part::manager() const
{
    Q_D(const Part);

    return d->m_manager;
}

Part *Part::hitTest(QWidget *widget, const QPoint &)
{
    Q_D(Part);

    if ((QWidget *)d->m_widget != widget) {
        return nullptr;
    }

    return this;
}

void Part::setWidget(QWidget *widget)
{
    Q_D(Part);
    d->m_widget = widget;
    connect(d->m_widget.data(), &QWidget::destroyed, this, &Part::slotWidgetDestroyed, Qt::UniqueConnection);
}

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 72)
void Part::setSelectable(bool selectable)
{
    Q_D(Part);

    d->m_bSelectable = selectable;
}

bool Part::isSelectable() const
{
    Q_D(const Part);

    return d->m_bSelectable;
}
#endif

void Part::customEvent(QEvent *ev)
{
    if (PartActivateEvent::test(ev)) {
        partActivateEvent(static_cast<PartActivateEvent *>(ev));
        return;
    }

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 72)
    if (PartSelectEvent::test(ev)) {
        partSelectEvent(static_cast<PartSelectEvent *>(ev));
        return;
    }
#endif

    if (GUIActivateEvent::test(ev)) {
        guiActivateEvent(static_cast<GUIActivateEvent *>(ev));
        return;
    }

    QObject::customEvent(ev);
}

void Part::partActivateEvent(PartActivateEvent *)
{
}

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 72)
void Part::partSelectEvent(PartSelectEvent *)
{
}
#endif

void Part::guiActivateEvent(GUIActivateEvent *)
{
}

QWidget *Part::hostContainer(const QString &containerName)
{
    if (!factory()) {
        return nullptr;
    }

    return factory()->container(containerName, this);
}

void Part::slotWidgetDestroyed()
{
    Q_D(Part);

    d->m_widget = nullptr;
    if (d->m_autoDeletePart) {
        // qCDebug(KPARTSLOG) << "deleting part" << objectName();
        this->deleteLater();
    }
}

#if KPARTS_BUILD_DEPRECATED_SINCE(5, 90)
void Part::loadPlugins()
{
    Q_D(Part);

    PartBase::loadPlugins(this, this, d->m_metaData.pluginId());
}
#endif

void Part::setMetaData(const KPluginMetaData &metaData)
{
    Q_D(Part);

    d->m_metaData = metaData;
#if KPARTS_BUILD_DEPRECATED_SINCE(5, 77)
    QT_WARNING_PUSH
    QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
    QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
    d->PartBasePrivate::setComponentData(KAboutData::fromPluginMetaData(metaData));

    // backward-compatible registration, usage deprecated
#if KCOREADDONS_BUILD_DEPRECATED_SINCE(5, 76)
    KAboutData::registerPluginData(d->componentData());
#endif
    QT_WARNING_POP
#endif

    KXMLGUIClient::setComponentName(metaData.pluginId(), metaData.name());
}

#include "moc_part.cpp"
