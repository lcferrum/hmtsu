/*
 * HMTsu
 * Copyright (C) 2013 Lcferrum
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DESKTOPTOOLS_H
#define DESKTOPTOOLS_H

#include <QSettings>

class DesktopFile {
private:
    QSettings* desktop;
    QString lang_full;
    QString lang_short;
public:
    DesktopFile();
    DesktopFile(const QString &path);
    ~DesktopFile();
    void Open(const QString &path);
    bool IfOpened() const;
    QString Path() const;
    bool DesktopKeyValue(const QString &key, bool locval, QString &value) const;
    static QString DesktopIconPath(const QString &icon_value);
};

#endif // DESKTOPTOOLS_H
