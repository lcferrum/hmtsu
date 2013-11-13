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

#include <QSettings>
#include <QTextCodec>
#include "iconprovider.h"
#include "desktoptools.h"

QString DesktopTools::lang="";

DesktopTools::DesktopTools():
    QAbstractListModel(NULL)
{
}

bool DesktopTools::DesktopKeyValue(const QString &fname, const QString &key, bool locval, QString &value)
{
    QSettings desktop(fname, QSettings::IniFormat);
    desktop.setIniCodec(QTextCodec::codecForName("UTF-8"));
    desktop.beginGroup("Desktop Entry");

    QVariant val=locval?desktop.value(QString("%1[%2]").arg(key, lang)):desktop.value(key);

    if (val.isValid()) {
        value=val.toString();
        return true;
    } else if (locval&&(val=desktop.value(QString("%1[%2]").arg(key, lang.left(2)))).isValid()) {
        value=val.toString();
        return true;
    } else if (locval&&(val=desktop.value(key)).isValid()) {
        value=val.toString();
        return true;
    } else {
        return false;
    }
}

QString DesktopTools::DesktopIconPath(const QString &icon_value)
{
    if (icon_value.length()>0)
        return (IconProvider::HasIcon(icon_value)?
                    "image://icon/":
                    (access(icon_value.toLocal8Bit().constData(), R_OK)?"image://theme/":"file://"))
               +icon_value;
    else
        return "";
}

void DesktopTools::SetDesktopLang(const QString &locale)
{
    lang=locale;
}
