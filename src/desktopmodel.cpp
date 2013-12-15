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
#include <MDesktopEntry>
#include "iconprovider.h"
#include "desktopmodel.h"

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

bool DesktopModel::PopulateList()
{
    if (!populated) {
        populated=true;
        signalModelBusy(true);
        SourceThread.start();
        return true;
    } else
        return false;
}

void DesktopModel::ReceiveEntry(QString name, QString icon_path, QString full_path)
{
    //DesktopDescription new_item(name, IconProvider::ConvertPath(icon_path), full_path);
    DesktopDescription new_item(name, icon_path, full_path);
    QList<DesktopDescription>::iterator target_it=qUpperBound(DesktopList.begin(), DesktopList.end(), new_item);
    int target_ix=target_it-DesktopList.begin();
    beginInsertRows(QModelIndex(), target_ix, target_ix);
    DesktopList.insert(target_it, new_item);
    endInsertRows();
}

void DesktopModel::FinishList()
{
    signalModelBusy(false);
}

void DesktopSource::run()
{
    QDir AppScreenDir;
    AppScreenDir.setFilter(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable|QDir::CaseSensitive);
    AppScreenDir.setNameFilters(QStringList()<<"*.desktop");
    AppScreenDir.setPath("/usr/share/applications/");

    foreach (const QFileInfo &file, AppScreenDir.entryInfoList()) {
        MDesktopEntry CurrentDesktop(file.absoluteFilePath());
        if (!CurrentDesktop.isValid()) continue;
        if (CurrentDesktop.notShowIn().contains("X-MeeGo")) continue;
        signalNewEntry(CurrentDesktop.name(), IconProvider::ConvertPath(CurrentDesktop.icon()), file.absoluteFilePath());    //"QPixmap: It is not safe to use pixmaps outside the GUI thread"
        //signalNewEntry(CurrentDesktop.name(), CurrentDesktop.icon(), file.absoluteFilePath());
    }

    signalSourceDepleted();
}
