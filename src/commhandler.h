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

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QPair>

class Intercommunication: public QObject {
    Q_OBJECT
private:
    enum cmm_ntfs { cmm_ntf_WARNING, cmm_ntf_INFO, cmm_ntf_SERROR };

    QList< QPair<QString, cmm_ntfs> > MessageQueue;
    bool started;
    bool istty;
    bool error_msgs_enabled;
    bool warning_msgs_enabled;
    bool general_msgs_enabled;
    QString err_msg;
    QString custom_icon;
    bool error;
    int exit_code;
    void Notify(QString msg, cmm_ntfs type);
public:
    Intercommunication();
    bool IfError();
    int GetExitCode();
    void AddSafeError(QString msg);
    void AddSafeWarning(QString msg);
    bool IfErrorMsgsDisabled();
    void DisableErrorMsgs();
    void DisableWarningMsgs();
    void DisableGeneralMsgs();

    //Functions exposed to QML:
    Q_INVOKABLE void SetCustomExitCode(int code);
    Q_INVOKABLE void SetCustomInfoIcon(QString icon);
    Q_INVOKABLE void Start();
    Q_INVOKABLE void AddError(QString msg);
    Q_INVOKABLE void AddWarning(QString msg);
    Q_INVOKABLE void AddInfo(QString msg);
signals:
    void signalError(QVariant msg);
};

class IntercomHandler {
private:
    static int ref_count;
protected:
    static Intercommunication *Intercom;
public:
    IntercomHandler();
    virtual ~IntercomHandler()=0;   //A trick to make class abstract (N.B.: destructor IS implemented and NEVER overloaded). Works only with destructors.
    static int GetExitCode();
    static void DisableErrorMsgs();
    static void DisableWarningMsgs();
    static void DisableGeneralMsgs();
};

class ScopedIntercomHandler: protected IntercomHandler {
public:
    Intercommunication* operator&() {
        return Intercom;
    }
};

#endif // MESSAGEHANDLER_H
