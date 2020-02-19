/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>
                      David Faure <faure@kde.org>

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
    WindowArgs(const QRect &_geometry, bool _fullscreen, bool _menuBarVisible,
               bool _toolBarsVisible, bool _statusBarVisible, bool _resizable);
    WindowArgs(int _x, int _y, int _width, int _height, bool _fullscreen,
               bool _menuBarVisible, bool _toolBarsVisible,
               bool _statusBarVisible, bool _resizable);

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
