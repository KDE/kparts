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

#include "browserarguments.h"

using namespace KParts;

namespace KParts
{

struct BrowserArgumentsPrivate {
    BrowserArgumentsPrivate()
    {
        doPost = false;
        redirectedRequest = false;
        lockHistory = false;
        newTab = false;
        forcesNewWindow = false;
    }
    QString contentType; // for POST
    bool doPost;
    bool redirectedRequest;
    bool lockHistory;
    bool newTab;
    bool forcesNewWindow;
};

}

BrowserArguments::BrowserArguments()
{
    softReload = false;
    trustedSource = false;
    d = nullptr; // Let's build it on demand for now
}

BrowserArguments::BrowserArguments(const BrowserArguments &args)
{
    d = nullptr;
    (*this) = args;
}

BrowserArguments &BrowserArguments::operator=(const BrowserArguments &args)
{
    if (this == &args) {
        return *this;
    }

    delete d; d = nullptr;

    softReload = args.softReload;
    postData = args.postData;
    frameName = args.frameName;
    docState = args.docState;
    trustedSource = args.trustedSource;

    if (args.d) {
        d = new BrowserArgumentsPrivate(* args.d);
    }

    return *this;
}

BrowserArguments::~BrowserArguments()
{
    delete d;
    d = nullptr;
}

void BrowserArguments::setContentType(const QString &contentType)
{
    if (!d) {
        d = new BrowserArgumentsPrivate;
    }
    d->contentType = contentType;
}

void BrowserArguments::setRedirectedRequest(bool redirected)
{
    if (!d) {
        d = new BrowserArgumentsPrivate;
    }
    d->redirectedRequest = redirected;
}

bool BrowserArguments::redirectedRequest() const
{
    return d ? d->redirectedRequest : false;
}

QString BrowserArguments::contentType() const
{
    return d ? d->contentType : QString();
}

void BrowserArguments::setDoPost(bool enable)
{
    if (!d) {
        d = new BrowserArgumentsPrivate;
    }
    d->doPost = enable;
}

bool BrowserArguments::doPost() const
{
    return d ? d->doPost : false;
}

void BrowserArguments::setLockHistory(bool lock)
{
    if (!d) {
        d = new BrowserArgumentsPrivate;
    }
    d->lockHistory = lock;
}

bool BrowserArguments::lockHistory() const
{
    return d ? d->lockHistory : false;
}

void BrowserArguments::setNewTab(bool newTab)
{
    if (!d) {
        d = new BrowserArgumentsPrivate;
    }
    d->newTab = newTab;
}

bool BrowserArguments::newTab() const
{
    return d ? d->newTab : false;
}

void BrowserArguments::setForcesNewWindow(bool forcesNewWindow)
{
    if (!d) {
        d = new BrowserArgumentsPrivate;
    }
    d->forcesNewWindow = forcesNewWindow;
}

bool BrowserArguments::forcesNewWindow() const
{
    return d ? d->forcesNewWindow : false;
}
