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

#include "windowargs.h"

#include <QRect>

using namespace KParts;

namespace KParts
{

class WindowArgsPrivate : public QSharedData
{
public:
    WindowArgsPrivate()
        : x(-1), y(-1), width(-1), height(-1),
          fullscreen(false),
          menuBarVisible(true),
          toolBarsVisible(true),
          statusBarVisible(true),
          resizable(true),
          lowerWindow(false),
          scrollBarsVisible(true)
    {
    }

    // Position
    int x;
    int y;
    // Size
    int width;
    int height;
    bool fullscreen; //defaults to false
    bool menuBarVisible; //defaults to true
    bool toolBarsVisible; //defaults to true
    bool statusBarVisible; //defaults to true
    bool resizable; //defaults to true

    bool lowerWindow; //defaults to false
    bool scrollBarsVisible; //defaults to true
};

}

WindowArgs::WindowArgs()
    : d(new WindowArgsPrivate)
{
}

WindowArgs::WindowArgs(const WindowArgs &args)
    : d(args.d)
{
}

WindowArgs::~WindowArgs()
{
}

WindowArgs &WindowArgs::operator=(const WindowArgs &args)
{
    if (this == &args) {
        return *this;
    }

    d = args.d;
    return *this;
}

WindowArgs::WindowArgs(const QRect &_geometry, bool _fullscreen, bool _menuBarVisible,
                       bool _toolBarsVisible, bool _statusBarVisible, bool _resizable)
    : d(new WindowArgsPrivate)
{
    d->x = _geometry.x();
    d->y = _geometry.y();
    d->width = _geometry.width();
    d->height = _geometry.height();
    d->fullscreen = _fullscreen;
    d->menuBarVisible = _menuBarVisible;
    d->toolBarsVisible = _toolBarsVisible;
    d->statusBarVisible = _statusBarVisible;
    d->resizable = _resizable;
    d->lowerWindow = false;
}

WindowArgs::WindowArgs(int _x, int _y, int _width, int _height, bool _fullscreen,
                       bool _menuBarVisible, bool _toolBarsVisible,
                       bool _statusBarVisible, bool _resizable)
    : d(new WindowArgsPrivate)
{
    d->x = _x;
    d->y = _y;
    d->width = _width;
    d->height = _height;
    d->fullscreen = _fullscreen;
    d->menuBarVisible = _menuBarVisible;
    d->toolBarsVisible = _toolBarsVisible;
    d->statusBarVisible = _statusBarVisible;
    d->resizable = _resizable;
    d->lowerWindow = false;
}

void WindowArgs::setX(int x)
{
    d->x = x;
}

int WindowArgs::x() const
{
    return d->x;
}

void WindowArgs::setY(int y)
{
    d->y = y;
}

int WindowArgs::y() const
{
    return d->y;
}

void WindowArgs::setWidth(int w)
{
    d->width = w;
}

int WindowArgs::width() const
{
    return d->width;
}

void WindowArgs::setHeight(int h)
{
    d->height = h;
}

int WindowArgs::height() const
{
    return d->height;
}

void WindowArgs::setFullScreen(bool fs)
{
    d->fullscreen = fs;
}

bool WindowArgs::isFullScreen() const
{
    return d->fullscreen;
}

void WindowArgs::setMenuBarVisible(bool visible)
{
    d->menuBarVisible = visible;
}

bool WindowArgs::isMenuBarVisible() const
{
    return d->menuBarVisible;
}

void WindowArgs::setToolBarsVisible(bool visible)
{
    d->toolBarsVisible = visible;
}

bool WindowArgs::toolBarsVisible() const
{
    return d->toolBarsVisible;
}

void WindowArgs::setStatusBarVisible(bool visible)
{
    d->statusBarVisible = visible;
}

bool WindowArgs::isStatusBarVisible() const
{
    return d->statusBarVisible;
}

void WindowArgs::setResizable(bool resizable)
{
    d->resizable = resizable;
}

bool WindowArgs::isResizable() const
{
    return d->resizable;
}

void WindowArgs::setLowerWindow(bool lower)
{
    d->lowerWindow = lower;
}

bool WindowArgs::lowerWindow() const
{
    return d->lowerWindow;
}

void WindowArgs::setScrollBarsVisible(bool visible)
{
    d->scrollBarsVisible = visible;
}

bool WindowArgs::scrollBarsVisible() const
{
    return d->scrollBarsVisible;
}

