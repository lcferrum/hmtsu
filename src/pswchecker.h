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
#include <pwd.h>
#include "runmodes.h"
#include "commhandler.h"
#include "context.h"

class PswChecker: public QObject, protected IntercomHandler {
    Q_OBJECT
private:
    bool prepared;
    passwd *user_record;
    void Prepare(RunModes::QmlEnum mode, QString &target_user);
    bool CheckSuNoPass();
    bool CheckSudoNoPass();
public:
    PswChecker();

    //Functions exposed to QML:
    Q_INVOKABLE void Prepare(/* RunModes::QmlEnum */ int mode, QString target_user) {   //Non-local Q_ENUMS can't be used in Q_INVOKABLE - use this hack
        Prepare(static_cast<RunModes::QmlEnum>(mode), target_user);                     //Should be fixed in Qt5
    }
    Q_INVOKABLE void PswCheck(QString psw);
signals:
    void signalPswOk();
    void signalPswBad();
    void signalNoPsw();
};

#endif // PSWCHECKER_H
