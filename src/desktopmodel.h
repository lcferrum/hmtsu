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

#ifndef DESKTOPMODEL_H
#define DESKTOPMODEL_H

#include <QAbstractListModel>
#include <QThread>
#include <QList>

struct DesktopDescription {
    QString name;
    QString icon_path;
    QString full_path;
    DesktopDescription (const QString &name, const QString &icon_path, const QString &full_path):
        name(name), icon_path(icon_path), full_path(full_path) {}
    bool operator<(const DesktopDescription &var) const {
        return name.localeAwareCompare(var.name)<0;
    }
};

class DesktopSource: public QThread {
    Q_OBJECT
private:
    //Overloaded functions:
    void run();
signals:
    void signalNewEntry(QString name, QString icon_path, QString full_path);
    void signalSourceDepleted();
};


class DesktopModel: public QAbstractListModel {
    Q_OBJECT
private:
    QList<DesktopDescription> DesktopList;
    DesktopSource SourceThread;
    bool populated;
public:
    DesktopModel();
    ~DesktopModel();

    //ListModel's standart functions implemetation:
    Q_INVOKABLE QVariant get(int index);

    //Overloaded functions:
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

    //Other functions exposed to QML:
    Q_INVOKABLE bool PopulateList();
signals:
    void signalListPopulated();
public slots:
    void RecieveEntry(QString name, QString icon_path, QString full_path);
    void FinishList();
};

#endif // DESKTOPMODEL_H
