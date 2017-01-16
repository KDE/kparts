/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
             (C) 1999-2005 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "part.h"
#include "part_p.h"

#include "partactivateevent.h"
#include "partselectevent.h"
#include "guiactivateevent.h"
#include "partmanager.h"

#include <kxmlguifactory.h>
#include <kiconloader.h>


using namespace KParts;

Part::Part(QObject *parent)
    : QObject(parent), PartBase(*new PartPrivate(this))
{
    PartBase::setPartObject(this);
}

Part::Part(PartPrivate &dd, QObject *parent)
    : QObject(parent), PartBase(dd)
{
    PartBase::setPartObject(this);
}

Part::~Part()
{
    Q_D(Part);

    //qDebug() << this;

    if (d->m_widget) {
        // We need to disconnect first, to avoid calling it !
        disconnect(d->m_widget.data(), &QWidget::destroyed,
                   this, &Part::slotWidgetDestroyed);
    }

    if (d->m_manager) {
        d->m_manager->removePart(this);
    }

    if (d->m_widget && d->m_autoDeleteWidget) {
        // qDebug() << "deleting widget" << d->m_widget << d->m_widget->objectName();
        delete static_cast<QWidget *>(d->m_widget);
    }

    delete d->m_iconLoader;
}

void Part::embed(QWidget *parentWidget)
{
    if (widget()) {
        widget()->setParent(parentWidget, nullptr);
        widget()->setGeometry(0, 0, widget()->width(), widget()->height());
        widget()->show();
    }
}

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

KIconLoader *Part::iconLoader()
{
    Q_D(Part);

    if (!d->m_iconLoader) {
        d->m_iconLoader = new KIconLoader(componentName());
    }
    return d->m_iconLoader;
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
    connect(d->m_widget.data(), &QWidget::destroyed,
            this, &Part::slotWidgetDestroyed, Qt::UniqueConnection);
}

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

void Part::customEvent(QEvent *ev)
{
    if (PartActivateEvent::test(ev)) {
        partActivateEvent(static_cast<PartActivateEvent *>(ev));
        return;
    }

    if (PartSelectEvent::test(ev)) {
        partSelectEvent(static_cast<PartSelectEvent *>(ev));
        return;
    }

    if (GUIActivateEvent::test(ev)) {
        guiActivateEvent(static_cast<GUIActivateEvent *>(ev));
        return;
    }

    QObject::customEvent(ev);
}

void Part::partActivateEvent(PartActivateEvent *)
{
}

void Part::partSelectEvent(PartSelectEvent *)
{
}

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
        // qDebug() << "deleting part" << objectName();
        this->deleteLater();
    }
}

void Part::loadPlugins()
{
    PartBase::loadPlugins(this, this, componentData());
}

#include "moc_part.cpp"
