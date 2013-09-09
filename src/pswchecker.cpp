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
