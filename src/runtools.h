#ifndef RUNTOOLS_H
#define RUNTOOLS_H

#include <QStringList>
#include "commhandler.h"

class RunTools: protected IntercomHandler {
private:
    bool no_pass;
    pid_t Fork(int &master_fd, char **cmd, const QString &mode_dsc);
    virtual bool CustomForkAction();
    char* DropNewLine(char* in_buf, int buf_size);
    bool WaitForNoEcho(int &master_fd, bool immediate);
    QString GetProcName(pid_t pid);
protected:
    QString psw;
    void ClearPsw();
    QString QuotedJoin(const QStringList &list);
    bool Launch(char **cmd, const QString &path);
    char** StringListToArray(QStringList &list);
    void DeleteStringArray(char** arr);
public:
    RunTools(const QString &psw, bool no_pass);
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command)=0;
};

class SuRunTools: public RunTools {
public:
    SuRunTools(const QString &psw, bool no_pass):
        RunTools(psw, no_pass) {}
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command);
};

class SudoRunTools: public RunTools {
public:
    SudoRunTools(const QString &psw, bool no_pass):
        RunTools(psw, no_pass) {}
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command);
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
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command);
};

class PrintRunTools: public RunTools {
public:
    PrintRunTools(const QString &psw, bool no_pass):
        RunTools(psw, no_pass) {}
    virtual void Run(const QString &user, bool login, bool kpp_env, const QStringList &command);
};

#endif // RUNTOOLS_H
