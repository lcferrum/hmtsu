#ifndef MODESMODEL_H
#define MODESMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "runmodes.h"
#include "commhandler.h"

struct ModePair {
    QString first;
    RunModes::QmlEnum second;
    ModePair(QString var1, RunModes::QmlEnum var2):
        first(var1), second(var2) {}
    bool operator==(const ModePair &var) const {
        return second==var.second;
    }
};

class ModeValidator: public QAbstractListModel, protected IntercomHandler {
    Q_OBJECT
private:
    QList<ModePair> AvailableModes;
    int ini_idx;
public:
    ModeValidator(RunModes::QmlEnum mode, bool skip);

    //ListModel's standart functions implemetation:
    Q_INVOKABLE QVariant get(int index);
    Q_PROPERTY(int count READ rowCount CONSTANT)

    //Overloaded functions:
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

    //Other functions exposed to QML:
    Q_INVOKABLE int GetInitialIndex();
};

#endif // MODESMODEL_H
