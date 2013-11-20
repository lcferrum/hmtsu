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

#ifndef USERVALIDATOR_H
#define USERVALIDATOR_H

#include <QAbstractListModel>
#include <QStringList>
#include "commhandler.h"

class UsersModel: public QAbstractListModel, protected IntercomHandler {
    Q_OBJECT
private:
    QStringList AvailableUsers;
    int ini_idx;
public:
    UsersModel(QString user, bool skip);

    //ListModel's standart functions implemetation:
    Q_INVOKABLE QVariant get(int index);
    Q_PROPERTY(int count READ rowCount CONSTANT)

    //Overloaded functions:
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

    //Other functions exposed to QML:
    Q_INVOKABLE int GetInitialIndex();
};


#endif // USERVALIDATOR_H
