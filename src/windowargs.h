/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 1999 Simon Hausmann <hausmann@kde.org>
    SPDX-FileCopyrightText: 1999 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef __kparts_windowargs_h__
#define __kparts_windowargs_h__

#include <kparts/kparts_export.h>

#include <QSharedDataPointer>

class QRect;

namespace KParts
{
class WindowArgsPrivate;

/**
 * @class WindowArgs windowargs.h <KParts/WindowArgs>
 *
 * @short The WindowArgs are used to specify arguments to the "create new window"
 * call (see the createNewWindow variant that uses WindowArgs).
 * The primary reason for this is the javascript window.open function.
 */
class KPARTS_EXPORT WindowArgs
{
public:
    WindowArgs();
    ~WindowArgs();
    WindowArgs(const WindowArgs &args);
    WindowArgs &operator=(const WindowArgs &args);
    WindowArgs(const QRect &_geometry, bool _fullscreen, bool _menuBarVisible, bool _toolBarsVisible, bool _statusBarVisible, bool _resizable);
    WindowArgs(int _x, int _y, int _width, int _height, bool _fullscreen, bool _menuBarVisible, bool _toolBarsVisible, bool _statusBarVisible, bool _resizable);

    void setX(int x);
    int x() const;

    void setY(int y);
    int y() const;

    void setWidth(int w);
    int width() const;

    void setHeight(int h);
    int height() const;

    void setFullScreen(bool fs);
    bool isFullScreen() const;

    void setMenuBarVisible(bool visible);
    bool isMenuBarVisible() const;

    void setToolBarsVisible(bool visible);
    bool toolBarsVisible() const;

    void setStatusBarVisible(bool visible);
    bool isStatusBarVisible() const;

    void setResizable(bool resizable);
    bool isResizable() const;

    void setLowerWindow(bool lower);
    bool lowerWindow() const;

    void setScrollBarsVisible(bool visible);
    bool scrollBarsVisible() const;

private:
    QSharedDataPointer<WindowArgsPrivate> d;
};

}

#endif
