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
    QObject(NULL), prepared(false), no_pass(false), pw_passwd()
{
}

QVariant PswTools::PrepareForCheck(RunModes::QmlEnum mode, const QString &target_user)
{
    if (!prepared) {
        passwd *user_record;

        do {
            errno=0;
            user_record=(mode==RunModes::SU)?
                        getpwnam(target_user.toLocal8Bit().constData()):
                        getpwuid(mode==RunModes::ARIADNE?ROOT_UID:getuid());
        } while (errno==EINTR);

        if (!user_record) {
            Intercom->AddError(QCoreApplication::translate("Messages", "__pswchecker_err__"));
            return QVariant();
        }

        if (mode==RunModes::SU)
            CheckSuNoPass();

        if (mode==RunModes::SUDO)
            if (!CheckSudoNoPass()) {
                Intercom->AddError(QCoreApplication::translate("Messages", "__pswchecker_not_sudoer_err__"));
                return QVariant();
            }

        pw_passwd=user_record->pw_passwd;
        prepared=true;
        return QVariant(no_pass);
    } else
        return QVariant();
}

bool PswTools::CheckSuNoPass()
{
    //if (user_record->pw_uid==getuid()||!getuid())
    if (getuid()==ROOT_UID) {
        no_pass=true;
    }

    return true;
}

bool PswTools::CheckSudoNoPass()
{
    QProcess sudo;
    sudo.setStandardInputFile("/dev/null");
    sudo.start("/usr/bin/sudo", QStringList()<<"-S"<<"-v");
    sudo.waitForFinished();
    QByteArray output=sudo.readAllStandardError();

    if (!sudo.exitCode()) {
        no_pass=true;
    }

    return output.length()==0||output.endsWith(":\n");
}

QVariant PswTools::PswCheck(const QString &psw)
{
    if (prepared) {
        if (no_pass) {
            return QVariant(true);
        } else {
            char *psw_hash=NULL;

            psw_hash=crypt(psw.toLocal8Bit().constData(), pw_passwd.constData());

            if (!psw_hash) {
                Intercom->AddError(QCoreApplication::translate("Messages", "__pswchecker_err__"));
            } else {
                if (strcmp(pw_passwd.constData(), psw_hash))
                    return QVariant(false);
                else
                    return QVariant(psw);
            }
        }
    }
    return QVariant();
}

void PswTools::ClearPsw(QString &psw)
{
    psw.fill('\0');
    psw.clear();
}
