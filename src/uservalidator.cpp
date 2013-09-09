#include <pwd.h>
#include <QCoreApplication>
#include "uservalidator.h"

UserValidator::UserValidator(QString user, bool skip):
    QAbstractListModel(NULL)
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole]="name";
    setRoleNames(roles);

    if (skip||Intercom->IfError()) return;

    struct passwd *pw;

    setpwent();
    while ((pw=getpwent()))
        AvailableUsers.append(QString::fromLocal8Bit(pw->pw_name));
    endpwent();

    AvailableUsers.sort();

    if ((ini_idx=AvailableUsers.indexOf(user))==-1)
        Intercom->AddError(QCoreApplication::translate("Messages", "__uservalidator_wronguser_err%1__").arg(user));
}

int UserValidator::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()?0:AvailableUsers.count();
}

QVariant UserValidator::data(const QModelIndex &index, int role) const
{
    if (index.row()<0||index.row()>=AvailableUsers.count())
        return QVariant();

    if (role==Qt::DisplayRole)
        return AvailableUsers[index.row()];

    return QVariant();
}

QVariant UserValidator::get(int index)
{
    if (index<0||index>=AvailableUsers.count())
        return QVariant();

    QMap<QString, QVariant> ReturnItem;
    ReturnItem.insert("name", QVariant(AvailableUsers[index]));

    return QVariant(ReturnItem);
}

int UserValidator::GetInitialIndex()
{
    return ini_idx;
}
