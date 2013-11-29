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

#ifndef MODESMODEL_H
#define MODESMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "runmodes.h"
#include "commhandler.h"

struct ModePair {
    QString first;
    RunModes::QmlEnum second;
    ModePair(const QString &first, RunModes::QmlEnum second):
        first(first), second(second) {}
    bool operator==(const ModePair &var) const {
        return second==var.second;
    }
};

class ModesModel: public QAbstractListModel, protected IntercomHandler {
    Q_OBJECT
private:
    QList<ModePair> AvailableModes;
    bool populated;
    int Find(RunModes::QmlEnum mode);
public:
    ModesModel();

    //ListModel's standard functions implemetation:
    Q_INVOKABLE QVariant get(int index);
    Q_PROPERTY(int count READ rowCount CONSTANT)

    //Overloaded functions:
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

    //Other functions exposed to QML:
    Q_INVOKABLE int Find(/* RunModes::QmlEnum */ int mode) {    //Non-local Q_ENUMS can't be used in Q_INVOKABLE - use this hack
        return Find(static_cast<RunModes::QmlEnum>(mode));      //Should be fixed in Qt5
    }
    Q_INVOKABLE bool PopulateList();
};

#endif // MODESMODEL_H
