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

#ifndef RUNTOOLS_H
#define RUNTOOLS_H

#include <QStringList>
#include "commhandler.h"

class RunTools: protected IntercomHandler {
private:
    bool no_pass;
    pid_t Fork(int &master_fd, char **cmd, const QString &mode_dsc);
    virtual bool CustomForkAction();
    const char* DropNewLine(const char *in_buf, int buf_size);
    bool WaitForNoEcho(int &master_fd, bool immediate);
    QString GetProcName(pid_t pid);
    bool TryToShowSplash(pid_t pid, const QString &splash, const QString &splash_lscape);
protected:
    QString psw;
    QString QuotedJoin(const QStringList &list);
    bool Launch(char **cmd, const QString &path, const QString &splash, const QString &splash_lscape);
    char** StringListToArray(const QStringList &list);
    void DeleteStringArray(char **arr);
    void ClearPsw();
public:
    RunTools(const QString &psw, bool no_pass);
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command, const QString &splash, const QString &splash_lscape)=0;
};

class SuRunTools: public RunTools {
public:
    SuRunTools(const QString &psw, bool no_pass):
        RunTools(psw, no_pass) {}
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command, const QString &splash, const QString &splash_lscape);
};

class SudoRunTools: public RunTools {
public:
    SudoRunTools(const QString &psw, bool no_pass):
        RunTools(psw, no_pass) {}
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command, const QString &splash, const QString &splash_lscape);
};

class AriadneRunTools: public RunTools {
private:
    bool login;
    bool kpp_env;
    QString user;
    virtual bool CustomForkAction();
public:
    AriadneRunTools(const QString &psw, bool no_pass):
        RunTools(psw, no_pass) {}
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command, const QString &splash, const QString &splash_lscape);
};

class PrintRunTools: public RunTools {
public:
    PrintRunTools(const QString &psw, bool no_pass):
        RunTools(psw, no_pass) {}
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command, const QString &splash, const QString &splash_lscape);
};

#endif // RUNTOOLS_H
