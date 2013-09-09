#ifndef USERVALIDATOR_H
#define USERVALIDATOR_H

#include <QAbstractListModel>
#include <QStringList>
#include "commhandler.h"

class UserValidator: public QAbstractListModel, protected IntercomHandler {
    Q_OBJECT
private:
    QStringList AvailableUsers;
    int ini_idx;
public:
    UserValidator(QString user, bool skip);

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
