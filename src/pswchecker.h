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
