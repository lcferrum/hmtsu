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
#include <QStringList>
#include <QDir>
#include "iconprovider.h"
#include "desktoptools.h"

#define APP_SCREEN_PATH     "/usr/share/applications/"

QString DesktopTools::lang="";

DesktopTools::DesktopTools():
    QAbstractListModel(NULL),
    DesktopList()
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

int DesktopTools::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:DesktopList.count();
}

QVariant DesktopTools::data(const QModelIndex &index, int role) const
{
    if (index.row()<0||index.row()>=DesktopList.count())
        return QVariant();

    if (role==Qt::DisplayRole)
        return DesktopList[index.row()].name;
    else if (role==Qt::UserRole+1)
        return DesktopIconPath(DesktopList[index.row()].icon_path);
    else if (role==Qt::UserRole+2)
        return APP_SCREEN_PATH+DesktopList[index.row()].full_path;

    return QVariant();
}

QVariant DesktopTools::get(int index)
{
    if (index<0||index>=DesktopList.count())
        return QVariant();

    QMap<QString, QVariant> ReturnItem;
    ReturnItem.insert("name", QVariant(DesktopList[index].name));
    ReturnItem.insert("icon", QVariant(DesktopList[index].icon_path));
    ReturnItem.insert("path", QVariant(DesktopList[index].full_path));

    return QVariant(ReturnItem);
}

void DesktopTools::PrepareList()
{
    if (DesktopList.count()==0) {
        QDir AppScreenDir;
        AppScreenDir.setFilter(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::CaseSensitive);
        AppScreenDir.setNameFilters(QStringList()<<"*.desktop");
        AppScreenDir.setPath(APP_SCREEN_PATH);

        QString icon;
        QString name;
        QString path;

        foreach (const QString &file, AppScreenDir.entryList()) {
            path=APP_SCREEN_PATH+file;
            if (!DesktopKeyValue(path, "Name", true, name)) continue;
            if (!DesktopKeyValue(path, "Icon", true, icon)) continue;
            DesktopList.append(DesktopDescription(name, DesktopIconPath(icon), path));
        }
    }
}
