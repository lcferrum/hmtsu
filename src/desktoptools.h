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

#include <QAbstractListModel>

class DesktopTools: public QAbstractListModel {
    Q_OBJECT
private:
    static QString lang;
public:
    DesktopTools();
    static bool DesktopKeyValue(const QString &fname, const QString &key, bool locval, QString &value);
    static QString DesktopIconPath(const QString &icon_value);
    static void SetDesktopLang(const QString &locale);
};

#endif // DESKTOPTOOLS_H
