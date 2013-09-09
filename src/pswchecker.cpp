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

#include <errno.h>
#include <crypt.h>
#include <unistd.h>
#include <string.h>
#include <QCoreApplication>
#include <QProcess>
#include "pswchecker.h"

using namespace std;

Q_DECLARE_METATYPE(RunModes::QmlEnum)

PswChecker::PswChecker():
    QObject(NULL)
{
    prepared=false;
    user_record=NULL;
}

void PswChecker::Prepare(RunModes::QmlEnum mode, QString &target_user)
{
    if (!prepared) {
        do {
            errno=0;
            user_record=(mode==RunModes::SUDO)?
                        getpwuid(getuid()):
                        getpwnam(target_user.toLocal8Bit().constData());
        } while (errno==EINTR);

        if (!user_record) {
            Intercom->AddError(QCoreApplication::translate("Messages", "__pswchecker_err__"));
            return;
        }

        if (mode==RunModes::SU)
            CheckSuNoPass();

        if (mode==RunModes::SUDO)
            if (!CheckSudoNoPass()) {
                Intercom->AddError(QCoreApplication::translate("Messages", "__pswchecker_not_sudoer_err__"));
                return;
            }

        prepared=true;
    }
}

bool PswChecker::CheckSuNoPass()
{
    //if (user_record->pw_uid==getuid()||!getuid())
    if (!getuid())
        signalNoPsw();

    return true;
}

bool PswChecker::CheckSudoNoPass()
{
    QProcess Setsid;
    Setsid.setStandardInputFile("/dev/null");
    Setsid.start("/usr/bin/sudo", QStringList()<<"-S"<<"-v");
    Setsid.waitForFinished();
    QByteArray output=Setsid.readAllStandardError();

    if (!Setsid.exitCode())
        signalNoPsw();

    return output.length()==0||output.endsWith(":\n");
}

void PswChecker::PswCheck(QString psw)
{
    if (prepared) {
        char *psw_hash=NULL;

        psw_hash=crypt(psw.toLocal8Bit().constData(), user_record->pw_passwd);

        if (!psw_hash) {
            Intercom->AddError(QCoreApplication::translate("Messages", "__pswchecker_err__"));
        } else {
            if (strcmp(user_record->pw_passwd, psw_hash))
                signalPswBad();
            else
                signalPswOk();
        }
    }
}
