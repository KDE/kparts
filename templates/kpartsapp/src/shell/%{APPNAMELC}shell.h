/*
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .
 */

#ifndef %{APPNAMEUC}SHELL_H
#define %{APPNAMEUC}SHELL_H

// KF headers
#include <KParts/MainWindow>

namespace KParts {
class ReadWritePart;
}

/**
 * @short %{APPNAME} Shell
 */
class %{APPNAME}Shell : public KParts::MainWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    %{APPNAME}Shell();

    /**
     * Default Destructor
     */
    ~%{APPNAME}Shell() override;

    /**
     * Use this method to load whatever file/URL you have
     * @param url document to load
     */
    void loadDocument(const QUrl& url);

private Q_SLOTS:
    void fileNew();
    void fileOpen();

private:
    void setupActions();

private:
    KParts::ReadWritePart* m_part;
};

#endif // %{APPNAMEUC}_H
