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
#include "common.h"
#include "pswtools.h"

using namespace std;

Q_DECLARE_METATYPE(RunModes::QmlEnum)

PswTools::PswTools():
    QObject(NULL), prepared(false), user_record(NULL)
{
}

void PswTools::PrepareForCheck(RunModes::QmlEnum mode, const QString &target_user)
{
    if (!prepared) {
        do {
            errno=0;
            user_record=(mode==RunModes::SU)?
                        getpwnam(target_user.toLocal8Bit().constData()):
                        getpwuid(mode==RunModes::ARIADNE?ROOT_UID:getuid());
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

bool PswTools::CheckSuNoPass()
{
    //if (user_record->pw_uid==getuid()||!getuid())
    if (getuid()==ROOT_UID)
        signalNoPsw();

    return true;
}

bool PswTools::CheckSudoNoPass()
{
    QProcess sudo;
    sudo.setStandardInputFile("/dev/null");
    sudo.start("/usr/bin/sudo", QStringList()<<"-S"<<"-v");
    sudo.waitForFinished();
    QByteArray output=sudo.readAllStandardError();

    if (!sudo.exitCode())
        signalNoPsw();

    return output.length()==0||output.endsWith(":\n");
}

void PswTools::PswCheck(QString psw)
{
    if (prepared) {
        char *psw_hash=NULL;

        psw_hash=crypt(psw.toLocal8Bit().constData(), user_record->pw_passwd);
        ClearPsw(psw);

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

void PswTools::ClearPsw(QString &psw)
{
    psw.fill('\0');
    psw.clear();
}
