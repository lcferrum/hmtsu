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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <QString>
#include <QDeclarativeContext>
#include <QStringList>
#include "runtools.h"
#include "runmodes.h"
#include "commhandler.h"

class Context: public QObject, protected IntercomHandler {
    Q_OBJECT
private:
    enum ctx_msgs { ctx_msg_DESC, ctx_msg_FULL, ctx_msg_CMD, ctx_msg_NULL };
    enum ctx_acts { ctx_act_ASK_FOR_MORE, ctx_act_EXIT, ctx_act_CONTINUE };

    RunModes::QmlEnum run_mode;
    RunTools *Tools;
    ctx_acts action;
    ctx_msgs message;
    int verbosity;
    bool login;
    bool kpp_env;
    QString user;
    QString text;
    QStringList command;
    bool LoadTextFromDesktop(QString fname, QString key, QString lang);
    bool LoadCommandFromDesktop(QString fname);
    bool ToArgv(QString cmdline);
public:
    Context(int argc, char **argv, QString lang);
    bool IfExit();
    int GetVerboseLevel();
    void ActuallyRun();

    //Properties expose to QML
    Q_PROPERTY(RunModes::QmlEnum Mode READ CheckMode WRITE ChangeMode) //N.B.: non-local Q_ENUMS can't be used in Q_INVOKABLE, should be fixed in Qt5
    Q_PROPERTY(QString TargetUser READ GetTargetUser WRITE SetTargetUser)
    RunModes::QmlEnum CheckMode();
    void ChangeMode(RunModes::QmlEnum mode);
    QString GetTargetUser();
    void SetTargetUser(QString str);

    //Functions exposed to QML:
    Q_INVOKABLE bool GetLogin();
    Q_INVOKABLE bool GetPreserveEnv();
    Q_INVOKABLE void SetLogin(bool flag);
    Q_INVOKABLE void SetPreserveEnv(bool flag);
    Q_INVOKABLE bool IfCustomMessage();
    Q_INVOKABLE bool IfContinue();
    Q_INVOKABLE void SetCommand(QString str);
    Q_INVOKABLE QString GetText();
    Q_INVOKABLE void Run(QString psw, bool no_pass);
};

#endif // CONTEXT_H
