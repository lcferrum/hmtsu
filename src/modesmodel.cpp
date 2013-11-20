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

#include <unistd.h>
#include <QCoreApplication>
#include <QStringList>
#include <QProcess>
#include "modesmodel.h"

ModesModel::ModesModel(RunModes::QmlEnum mode, bool skip):
    QAbstractListModel(NULL),
    AvailableModes(), ini_idx(-1)
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole]="name";
    roles[Qt::UserRole+1]="mode";
    setRoleNames(roles);

    if (skip||Intercom->IfError()) return;

    if (!access("/bin/devel-su", R_OK))
        AvailableModes.append(ModePair("devel-su", RunModes::SU));
    if (!access("/usr/bin/sudo", R_OK)) {
        QProcess dpkg;
        dpkg.start("/usr/bin/dpkg", QStringList()<<"-s"<<"opensudo");
        if (dpkg.waitForFinished()&&!dpkg.exitCode())
            AvailableModes.append(ModePair("opensudo", RunModes::SUDO));
        else
            AvailableModes.append(ModePair("sudo", RunModes::SUDO));
    }
    if (!access("/usr/bin/ariadne", R_OK))
        AvailableModes.append(ModePair("ariadne", RunModes::ARIADNE));

    if (AvailableModes.count()<=0)
        Intercom->AddError(QCoreApplication::translate("Messages", "__ModesModel_nomodes_err__"));

    if ((ini_idx=AvailableModes.indexOf(ModePair("", mode)))==-1)
        Intercom->AddError(QCoreApplication::translate("Messages", "__ModesModel_wrongmode_err__"));
}

int ModesModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:AvailableModes.count();
}

QVariant ModesModel::data(const QModelIndex &index, int role) const
{
    if (index.row()<0||index.row()>=AvailableModes.count())
        return QVariant();

    if (role==Qt::DisplayRole)
        return AvailableModes[index.row()].first;
    else if (role==Qt::UserRole+1)
        return AvailableModes[index.row()].second;

    return QVariant();
}

QVariant ModesModel::get(int index)
{
    if (index<0||index>=AvailableModes.count())
        return QVariant();

    QMap<QString, QVariant> ReturnItem;
    ReturnItem.insert("name", QVariant(AvailableModes[index].first));
    ReturnItem.insert("mode", QVariant(AvailableModes[index].second));

    return QVariant(ReturnItem);
}

int ModesModel::GetInitialIndex()
{
    return ini_idx;
}
