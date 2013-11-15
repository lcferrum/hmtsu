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

#include <QLocale>
#include "iconprovider.h"
#include "desktoptools.h"

DesktopFile::DesktopFile():
    desktop(NULL), lang(QLocale::system().name())
{
}

DesktopFile::DesktopFile(const QString &path):
    desktop(NULL), lang(QLocale::system().name())
{
    Open(path);
}

DesktopFile::~DesktopFile()
{
    delete desktop;
}

void DesktopFile::Open(const QString &path)
{
    delete desktop;
    desktop=new QSettings(path, QSettings::IniFormat);
    desktop->setIniCodec("UTF-8");
    desktop->beginGroup("Desktop Entry");
}

bool DesktopFile::IfOpened() const
{
    return desktop;
}

QString DesktopFile::Path() const
{
    if (desktop) {
        return desktop->fileName();
    } else
        return "";
}


bool DesktopFile::DesktopKeyValue(const QString &key, bool locval, QString &value) const
{
    if (!desktop)
        return false;

    QVariant val=locval?desktop->value(QString("%1[%2]").arg(key, lang)):desktop->value(key);

    if (val.isValid()) {
        value=val.toString();
        return true;
    } else if (locval&&(val=desktop->value(QString("%1[%2]").arg(key, lang.left(2)))).isValid()) {
        value=val.toString();
        return true;
    } else if (locval&&(val=desktop->value(key)).isValid()) {
        value=val.toString();
        return true;
    } else {
        return false;
    }
}

QString DesktopFile::DesktopIconPath(const QString &icon_value)
{
    if (icon_value.length()>0)
        return (IconProvider::HasIcon(icon_value)?
                    "image://icon/":
                    (access(icon_value.toLocal8Bit().constData(), R_OK)?"image://theme/":"file://"))
               +icon_value;
    else
        return "";
}
