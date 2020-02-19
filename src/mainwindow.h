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
#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <kparts/part.h>

#include <kxmlguiwindow.h>

class QString;

namespace KParts
{

class MainWindowPrivate;

/**
 * @class MainWindow mainwindow.h <KParts/MainWindow>
 *
 * @short A KPart-aware main window, whose user interface is described in XML.
 *
 * It implements all internal interfaces in the case of a
 * KMainWindow as host: the builder and servant interface (for menu
 * merging).
 *
 * Inherit your main window from this class
 * and make sure to call @c setXMLFile() and @c setupGUI() before you
 * call @c createGUI() on the KPart.
 *
 * For example:
 * \code
 * setCentralWidget(m_part->widget());
 * setXMLFile(QStringLiteral("appui.rc"));
 * setupGUI(ToolBar | Keys | StatusBar | Save); // Never Create flag here
 * createGUI(m_part);
 * \endcode
 *
 * @warning You should not pass the @c Default flag set to @c setupGUI(),
 * since it contains the @c Create flag, which is not supposed to be used
 * from this class.
 * @see KXmlGuiWindow::Create, @see setupGUI, @see createGUI
 *
 */
class KPARTS_EXPORT MainWindow : public KXmlGuiWindow, virtual public PartBase
{
    Q_OBJECT
public:
    /**
     * Constructor, same signature as KMainWindow.
     */
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    /**
     * Destructor.
     */
    ~MainWindow() override;

public Q_SLOTS:
    void configureToolbars() override;

protected Q_SLOTS:

    /**
     * Create the GUI (by merging the host's and the active part's)
     * You _must_ call this in order to see any GUI being created.
     *
     * In a main window with multiple parts being shown (e.g. as in Konqueror)
     * you need to connect this slot to the
     * KPartManager::activePartChanged() signal
     *
     * @param part The active part (set to 0L if no part).
     */
    void createGUI(KParts::Part *part);

    /**
     * Enable or disable the automatic setting of window titles by the part's document title.
     * By default, a part always changes the window title when the document changes.
     * @note This value must be set before calling createGUI().
     *
     * @param enabled boolean to enable or disable the window title handling
     * @since 5.24
     */
    void setWindowTitleHandling(bool enabled);

    /**
     * Called when the active part wants to change the statusbar message
     * Reimplement if your mainwindow has a complex statusbar
     * (with several items)
     */
    virtual void slotSetStatusBarText(const QString &);

    /**
     * Rebuilds the GUI after KEditToolBar changed the toolbar layout.
     * @see configureToolbars()
     */
    void saveNewToolbarConfig() override;

protected:
    virtual void createShellGUI(bool create = true);

private:
    MainWindowPrivate *const d;
};

}

#endif
