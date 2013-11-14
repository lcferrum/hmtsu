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

#include <QStringList>
#include <QDir>
#include <QtAlgorithms>
#include "desktoptools.h"
#include "desktopmodel.h"

#define APP_SCREEN_PATH     "/usr/share/applications/"
#define THREAD_TIMEOUT      10000   //10 seconds

DesktopModel::DesktopModel():
    QAbstractListModel(NULL),
    DesktopList(), SourceThread(), populated(false)
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole]="name";
    roles[Qt::UserRole+1]="icon";
    roles[Qt::UserRole+2]="path";
    setRoleNames(roles);

    connect(&SourceThread, SIGNAL(signalNewEntry(QString, QString, QString)), this, SLOT(ReceiveEntry(QString, QString, QString)), Qt::QueuedConnection);
    connect(&SourceThread, SIGNAL(signalSourceDepleted()), this, SLOT(FinishList()), Qt::QueuedConnection);
}

DesktopModel::~DesktopModel()
{
    if(!SourceThread.wait(THREAD_TIMEOUT))
        SourceThread.terminate();
}

int DesktopModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:DesktopList.count();
}

QVariant DesktopModel::data(const QModelIndex &index, int role) const
{
    if (index.row()<0||index.row()>=DesktopList.count())
        return QVariant();

    if (role==Qt::DisplayRole)
        return DesktopList[index.row()].name;
    else if (role==Qt::UserRole+1)
        return DesktopList[index.row()].icon_path;
    else if (role==Qt::UserRole+2)
        return DesktopList[index.row()].full_path;

    return QVariant();
}

QVariant DesktopModel::get(int index)
{
    if (index<0||index>=DesktopList.count())
        return QVariant();

    QMap<QString, QVariant> ReturnItem;
    ReturnItem.insert("name", QVariant(DesktopList[index].name));
    ReturnItem.insert("icon", QVariant(DesktopList[index].icon_path));
    ReturnItem.insert("path", QVariant(DesktopList[index].full_path));

    return QVariant(ReturnItem);
}

bool DesktopModel::PopulateList()
{
    if (!populated) {
        populated=true;
        beginResetModel();
        SourceThread.start();
        return true;
    } else
        return false;
}

void DesktopModel::ReceiveEntry(QString name, QString icon_path, QString full_path)
{
    DesktopList.append(DesktopDescription(name, icon_path, full_path));
}

void DesktopModel::FinishList()
{
    qSort(DesktopList);
    endResetModel();
    signalListPopulated();
}

void DesktopSource::run()
{
    QDir AppScreenDir;
    DesktopFile CurrentDesktop;
    AppScreenDir.setFilter(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::CaseSensitive);
    AppScreenDir.setNameFilters(QStringList()<<"*.desktop");
    AppScreenDir.setPath(APP_SCREEN_PATH);

    QString icon, name, path;

    foreach (const QString &file, AppScreenDir.entryList()) {
        path=APP_SCREEN_PATH+file;
        CurrentDesktop.Open(path);
        if (CurrentDesktop.DesktopKeyValue("NotShowIn", false, name)&&name=="X-MeeGo") continue;
        if (!CurrentDesktop.DesktopKeyValue("Name", true, name)) continue;
        if (!CurrentDesktop.DesktopKeyValue("Icon", true, icon)) continue;
        signalNewEntry(name, DesktopFile::DesktopIconPath(icon), path); //"QPixmap: It is not safe to use pixmaps outside the GUI thread"
    }

    signalSourceDepleted();
}
