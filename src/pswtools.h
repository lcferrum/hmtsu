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

#ifndef PSWCHECKER_H
#define PSWCHECKER_H

#include <QObject>
#include <QByteArray>
#include <pwd.h>
#include "runmodes.h"
#include "commhandler.h"
#include "context.h"

class PswTools: public QObject, protected IntercomHandler {
    Q_OBJECT
private:
    bool prepared;
    bool no_pass;
    QByteArray pw_passwd;
    bool PrepareForCheck(RunModes::QmlEnum mode, const QString &target_user);
    void CheckSuNoPass();
    bool CheckSudoNoPass();
public:
    PswTools();

    //Functions exposed to QML:
    Q_INVOKABLE bool PrepareForCheck(/* RunModes::QmlEnum */ int mode, const QString &target_user) {    //Non-local Q_ENUMS can't be used in Q_INVOKABLE - use this hack
        return PrepareForCheck(static_cast<RunModes::QmlEnum>(mode), target_user);                      //Should be fixed in Qt5
    }
    Q_INVOKABLE QVariant PswCheck(const QString &psw);
    Q_INVOKABLE bool IfNoPass();
};

#endif // PSWCHECKER_H
