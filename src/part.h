/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
             (C) 1999 David Faure <faure@kde.org>

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

#ifndef _KPARTS_PART_H
#define _KPARTS_PART_H

#include <kparts/partbase.h>

class KIconLoader;
class QWidget;
class QEvent;
class QPoint;

/**
 * The KParts namespace,
 */
namespace KParts
{

class PartManager;
class PartPrivate;
class PartActivateEvent;
class PartSelectEvent;
class GUIActivateEvent;


/**
 * @class Part part.h <KParts/Part>
 *
 * @short Base class for parts.
 *
 * A "part" is a GUI component, featuring:
 * @li A widget embeddedable in any application.
 * @li GUI elements that will be merged in the "host" user interface
 * (menubars, toolbars... ).
 *
 * <b>About the widget:</b>\n
 *
 * Note that KParts::Part does not inherit QWidget.
 * This is due to the fact that the "visual representation"
 * will probably not be a mere QWidget, but an elaborate one.
 * That's why when implementing your KParts::Part (or derived)
 * you should call KParts::Part::setWidget() in your constructor.
 *
 * <b>About the GUI elements:</b>\n
 *
 * Those elements trigger actions, defined by the part ( action()).
 * The layout of the actions in the GUI is defined by an XML file ( setXMLFile()).
 *
 * See also ReadOnlyPart and ReadWritePart, which define the
 * framework for a "viewer" part and for an "editor"-like part.
 * Use Part directly only if your part doesn't fit into those.
 */
class KPARTS_EXPORT Part : public QObject, public PartBase
{
    Q_OBJECT

    KPARTS_DECLARE_PRIVATE(Part)

public:

    /**
     *  Constructor.
     *
     *  @param parent Parent object of the part.
     */
    explicit Part(QObject *parent = nullptr);

    /**
     *  Destructor.
     */
    ~Part() override;

    /**
     * Embed this part into a host widget.
     *
     * You don't need to do this if you created the widget with the
     * correct parent widget - this is just a QWidget::reparent().
     * Note that the Part is still the holder
     * of the QWidget, meaning that if you delete the Part,
     * then the widget gets destroyed as well, and vice-versa.
     * This method is not recommended since creating the widget with the correct
     * parent is simpler anyway.
     */
    virtual void embed(QWidget *parentWidget);

    /**
     * @return The widget defined by this part, set by setWidget().
     */
    virtual QWidget *widget();

    /**
     * @internal
     * Used by the part manager.
     */
    virtual void setManager(PartManager *manager);

    /**
     * Returns the part manager handling this part, if any (0L otherwise).
     */
    PartManager *manager() const;

    /**
     * By default, the widget is deleted by the part when the part is deleted.
     * The hosting application can call setAutoDeleteWidget(false) to
     * disable this behavior, given that the widget is usually deleted by
     * its parent widget anyway.
     * This is a method for the hosting application only, Part subclasses
     * should never call this.
     */
    void setAutoDeleteWidget(bool autoDeleteWidget);

    /**
     * By default, the part deletes itself when its widget is deleted.
     * The hosting application can call setAutoDeletePart(false) to
     * disable this behavior, to be able to delete the widget and then the part,
     * independently.
     * This is a method for the hosting application only, Part subclasses
     * should never call this.
     */
    void setAutoDeletePart(bool autoDeletePart);

    /**
     * Returns the part (this, or a child part) at the given global position.
     * This is called by the part manager to ask whether a part should be activated
     * when clicking somewhere. In most cases the default implementation is enough.
     * Reimplement this if your part has child parts in some areas (like in khtml or koffice)
     * @param widget the part widget being clicked - usually the same as widget(), except in koffice.
     * @param globalPos the mouse coordinates in global coordinates
     */
    virtual Part *hitTest(QWidget *widget, const QPoint &globalPos);

    /**
     *  @param selectable Indicates whether the part is selectable or not.
     */
    virtual void setSelectable(bool selectable);

    /**
     *  Returns whether the part is selectable or not.
     */
    bool isSelectable() const;

    /**
     * Use this icon loader to load any icons that are specific to this part,
     * i.e. icons installed into this part's own directories as opposed to standard
     * kde icons.
     *
     * Make sure to call setComponentData before calling iconLoader.
     */
    KIconLoader *iconLoader();

Q_SIGNALS:
    /**
     * Emitted by the part, to set the caption of the window(s)
     * hosting this part
     *
     * @note this signal has only an effect on the window title if window title
     *       handling is enabled @see KParts::MainWindow::setWindowTitleHandling
     */
    void setWindowCaption(const QString &caption);
    /**
     * Emitted by the part, to set a text in the statusbar of the window(s)
     * hosting this part
     */
    void setStatusBarText(const QString &text);

protected:
    /**
     * Set the main widget.
     *
     * Call this in the Part-inherited class constructor.
     */
    virtual void setWidget(QWidget *widget);

    /**
     * @internal
     */
    void customEvent(QEvent *event) override;

    /**
     * Convenience method which is called when the Part received a PartActivateEvent .
     * Reimplement this if you don't want to reimplement event and test for the event yourself
     * or even install an event filter.
     */
    virtual void partActivateEvent(PartActivateEvent *event);

    /**
     * Convenience method which is called when the Part received a
     * PartSelectEvent .
     * Reimplement this if you don't want to reimplement event and
     * test for the event yourself or even install an event filter.
     */
    virtual void partSelectEvent(PartSelectEvent *event);

    /**
     * Convenience method which is called when the Part received a
     * GUIActivateEvent .
     * Reimplement this if you don't want to reimplement event and
     * test for the event yourself or even install an event filter.
     */
    virtual void guiActivateEvent(GUIActivateEvent *event);

    /**
     * Convenience method for KXMLGUIFactory::container.
     * @return a container widget owned by the Part's GUI.
     */
    QWidget *hostContainer(const QString &containerName);

    /**
     * Load this part's plugins now.
     * Normally you want to call this at the end of the part constructor,
     * if you used setComponentData(componentData, false)
     * @since 4.1
     */
    void loadPlugins();
    using PartBase::loadPlugins;

protected Q_SLOTS:
    /**
     * @internal
     */
    void slotWidgetDestroyed();

protected:
    Part(PartPrivate &dd, QObject *parent);

private:
    Q_DISABLE_COPY(Part)
};

} // namespace

#endif
