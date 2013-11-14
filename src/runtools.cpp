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

#include <sys/wait.h>
#include <sys/ioctl.h>
#include <pty.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <utmp.h>
#include <termios.h>
#include <stdlib.h>
#include <pwd.h>
#include <iostream>
#include <QCoreApplication>
#include <QFile>
#include <QByteArray>
#include <QProcess>
#include "common.h"
#include "pswtools.h"
#include "runmodes.h"
#include "runtools.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#define NO_ECHO_TRIES       15      //15x0.001 seconds
#define OUTPUT_TIMEOUT      5       //5 seconds
#define PARENT_HANDICAP     250000  //0.25 seconds
#define BUFFER_SIZE         512     //512 bytes

RunTools::RunTools(const QString &psw, bool no_pass):
    no_pass(no_pass), psw(psw)
{
}

void PrintRunTools::Run(const QString &user, bool login, bool kpp_env, const QStringList &command, const QString &splash, const QString &splash_lscape)
{
    Q_UNUSED(user);
    Q_UNUSED(login);
    Q_UNUSED(kpp_env);
    Q_UNUSED(command);
    Q_UNUSED(splash);
    Q_UNUSED(splash_lscape);
    std::cout<<qPrintable(psw)<<std::endl;
    PswTools::ClearPsw(psw);
}

void SuRunTools::Run(const QString &user, bool login, bool kpp_env, const QStringList &command, const QString &splash, const QString &splash_lscape)
{
    QStringList args;
    char** cmd;
    const QString path="/bin/devel-su";

    args.append(path);
    if (login) args.append("-");
    args.append(user);
    if (kpp_env) args.append("-p");
    args.append("-c");
    args.append(QuotedJoin(command));

    IF_DEBUG(qDebug()<<"Tool [SU"<<(kpp_env?"kpp_env":"")<<(login?"login":"")<<"] command:"<<args;)

    cmd=StringListToArray(args);

    Launch(cmd, path, splash, splash_lscape);

    DeleteStringArray(cmd);
}

void SudoRunTools::Run(const QString &user, bool login, bool kpp_env, const QStringList &command, const QString &splash, const QString &splash_lscape)
{
    QStringList args;
    char** cmd;
    const QString path="/usr/bin/sudo";

    args.append(path);
    args.append("-S");
    if (!kpp_env&&!login) args.append("-H");
    if (login) args.append("-i");
    args.append("-u");
    args.append(user);
    if (!kpp_env||login) args.append("--");
    if (login) {
        args.append("-c");
        args.append(QuotedJoin(command));
    } else
        args.append(command);

    IF_DEBUG(qDebug()<<"Tool [SUDO"<<(kpp_env?"kpp_env":"")<<(login?"login":"")<<"] command:"<<args;)

    cmd=StringListToArray(args);

    Launch(cmd, path, splash, splash_lscape);

    DeleteStringArray(cmd);
}

void AriadneRunTools::Run(const QString &user, bool login, bool kpp_env, const QStringList &command, const QString &splash, const QString &splash_lscape)
{
    QStringList args;
    char** cmd;
    const QString path="/usr/bin/ariadne";

    this->login=login;
    this->kpp_env=kpp_env;
    this->user=user;

    args.append(path);
    args.append("-k");
    args.append("-u");
    args.append(user);
    args.append("--");
    args.append(command);

    if (login) {
        QString cmd_line=QuotedJoin(args);
        args.clear();
        args.append("/usr/bin/aegis-exec");
        args.append("-l");
        args.append("-u");
        args.append(user);
        args.append(cmd_line);
    }

    IF_DEBUG(qDebug()<<"Tool [ARIADNE"<<(kpp_env?"kpp_env":"")<<(login?"login":"")<<"] command:"<<args;)

    cmd=StringListToArray(args);

    Launch(cmd, path, splash, splash_lscape);

    DeleteStringArray(cmd);
}

bool RunTools::CustomForkAction()
{
    return true;
}

bool AriadneRunTools::CustomForkAction()
{
    passwd *user_record;

    do {
        errno=0;
        user_record=getpwnam(user.toLocal8Bit().constData());
    } while (errno==EINTR);

    if (!user_record) return false;

    if (!login&&!kpp_env) {
        setenv("HOME", user_record->pw_dir, true);
        if (user_record->pw_uid!=ROOT_UID) {
            setenv("USER", user_record->pw_name, true);
            setenv("LOGNAME", user_record->pw_name, true);
        }
    }

    return true;
}

pid_t RunTools::Fork(int &master_fd, char **cmd, const QString &mode_dsc)
{
    pid_t pid;
    winsize argp;
    int slave_fd=-1;

    if (openpty(&master_fd, &slave_fd, NULL, NULL, NULL)==-1) {
        Intercom->AddSafeError(QCoreApplication::translate("Messages", "__runtools_fork_err_BASE%1%2__").arg(mode_dsc, strerror(errno)));
        return 0;
    }

    if (!ioctl(STDOUT_FILENO, TIOCGWINSZ, &argp)) { //If stdout is connected to some tty - set same window size for pty
        ioctl(slave_fd, TIOCSWINSZ, &argp);
    }

    pid=fork();
    switch (pid) {
        case -1:
            Intercom->AddSafeError(QCoreApplication::translate("Messages", "__runtools_fork_err_BASE%1%2__").arg(mode_dsc, strerror(errno)));
            close(master_fd);
            close(slave_fd);
            return 0;
        case 0:
            if (!CustomForkAction()||close(master_fd)==-1||login_tty(slave_fd)==-1||execvp(cmd[0], cmd)) {   //if close master_fd now here - exits as soon as parent closes his master_fd (or dies)
                DeleteStringArray(cmd);
                std::cerr<<"hmtsu: "<<strerror(errno)<<std::endl;
                _exit(ERROR_EXIT_CODE);
            }
        default:
            close(slave_fd);
            return pid;
    }
}

bool RunTools::Launch(char **cmd, const QString &path, const QString &splash, const QString &splash_lscape)
{
    pid_t pid;
    int master_fd=-1;

    if ((pid=Fork(master_fd, cmd, path))) {
        fd_set rfds;
        timeval tv;
        char rbuf[BUFFER_SIZE]={};
        int ret, status;
        bool first_pass=true;
        bool password_needed=!no_pass;

        IF_DEBUG(qDebug()<<"Tool pid is"<<pid;)

        usleep(PARENT_HANDICAP);
        forever {
            FD_ZERO(&rfds);                                         //Value of fd_set after timeout or error is undefined -> reset every iteration
            FD_SET(master_fd, &rfds);
            tv.tv_sec=OUTPUT_TIMEOUT;                               //Select modifies timeval on Linux -> reset every iteration
            tv.tv_usec=0;
            ret=select(FD_SETSIZE, &rfds, NULL, NULL, &tv);         //Wait for possible stdout/stderr output from child (if child dies during select: behaviour is undefined - always check for timeout)
            if (ret==0) {                                           //Timeout -> check cause
                IF_DEBUG(qDebug()<<"Select timeouted";)
                //if (WaitForNoEcho(master_fd, true)&&GetProcName(pid).startsWith(path)) {    //Child unexpectedly asks for password -> display error & abort
                if (WaitForNoEcho(master_fd, true)) {               //Child unexpectedly asks for password -> display error & abort
                    Intercom->AddSafeError(QCoreApplication::translate("Messages", "__runtools_fork_err_BADPSW%1__").arg(path));
                    close(master_fd);
                    return false;
                } else {                                            //Check if child is dead
                    status=0;
                    ret=waitpid(pid, &status, WNOHANG);             //Checks if child alive without blocking process (not affected by INT signal)
                    if (ret==0) continue;                           //Child is still alive but silent -> continue
                        else break;                                 //Child already dead or something went wrong with wait -> return as normal
                }
            } else if (ret==-1&&errno==EINTR) {                     //INT signal caught -> continue
                continue;
            } else if (ret==-1) {                                   //System error encountered while waiting -> display error & abort
                Intercom->AddSafeError(QCoreApplication::translate("Messages", "__runtools_fork_err_BASE%1%2__").arg(path, strerror(errno)));
                close(master_fd);
                return false;
            }

            ret=read(master_fd, rbuf, BUFFER_SIZE-1);               //Read data from child stdout/stderr
            if (ret==0||(ret==-1&&errno==EIO)) {                    //EOF or EIO recieved -> prepare to return
                IF_DEBUG(qDebug()<<(ret?"EIO":"EOF")<<"received from pty";)
                do {                                                //Wait till child dies if it's still alive
                    status=0;
                    ret=waitpid(pid, &status, 0);
                } while (ret==-1&&errno==EINTR);                    //Tries wait again if it crashed because of INT signal
                break;                                              //Child dead or something went wrong with wait -> return as normal
            } else if (ret==-1&&errno==EINTR) {                     //INT signal caught -> continue
                continue;
            } else if (ret==-1) {                                   //System error encountered while reading -> display error & abort
                Intercom->AddSafeError(QCoreApplication::translate("Messages", "__runtools_fork_err_BASE%1%2__").arg(path, strerror(errno)));
                close(master_fd);
                return false;
            }
            rbuf[ret]='\0';                                         //Prepare read data
            IF_DEBUG(qDebug()<<ret<<"bytes received from pty";)

            if (first_pass&&password_needed) {                      //If first pass and we need password -> wait for child to be ready for password
                if (WaitForNoEcho(master_fd, false)) {              //Child is ready for password -> print it to stdin
                    write(master_fd, psw.toLocal8Bit().constData(), psw.toLocal8Bit().length()+1);
                    write(master_fd, "\n", 1);
                    TryToShowSplash(pid, splash, splash_lscape);    //Try to show splash screen
                    IF_DEBUG(qDebug()<<"Password sent";)
                } else {                                            //Child was not ready for password -> just warn about it and print out data recieved from child
                    Intercom->AddSafeWarning(QCoreApplication::translate("Messages", "__runtools_fork_wrn_NOPSW%1__").arg(path));
                    std::cout<<rbuf;
                    password_needed=false;
                }
                PswTools::ClearPsw(psw);                            //Get rid of password copy in memory
            } else if (password_needed) {                           //If password was needed in previous pass -> print out data from child omitting leading new line
                std::cout<<DropNewLine(rbuf, ret);
                password_needed=false;
            } else {
                std::cout<<rbuf;                                    //Print out data recieved from child
            }

            first_pass=false;                                       //It's no longer a first pass
        }
        IF_DEBUG(qDebug()<<"Tool exited";)

        if (ret==pid||(ret==-1&&errno==ECHILD)) {                   //Child is dead -> set exit code (last ret/errno/status can be only from waitpid)
            Intercom->SetCustomExitCode(WIFEXITED(status)?WEXITSTATUS(status):-1);

            if (Intercom->GetExitCode()>0)                          //Child returned with error exit code -> warn about it
                Intercom->AddSafeWarning(QCoreApplication::translate("Messages", "__runtools_fork_wrn_ECODE%1%2%3__").arg(path).arg(Intercom->GetExitCode()).arg(QString::fromLocal8Bit(rbuf)));

            close(master_fd);
            return true;
        } else {                                                    //Failed to wait for child death -> display error
            Intercom->AddSafeError(QCoreApplication::translate("Messages", "__runtools_fork_err_BASE%1%2__").arg(path, strerror(errno)));
            close(master_fd);
            return false;
        }
    } else
        return false;
}

bool RunTools::TryToShowSplash(pid_t pid, const QString &splash, const QString &splash_lscape)
{
    if (splash.length()==0||isatty(STDIN_FILENO))  //STDIN is not connected to terminal if HMTsu launched with invoker or from QtCreator
        return false;

    usleep(PARENT_HANDICAP);

    //Quick and dirty way to guess PID of actual process (and not the PID of devel-su/sudo/ariadne):
    //  Pgrep util searches for the last child of original process or the child with multiple children
    //  If found - this child is assumed to be the actual process
    QProcess Pgrep;
    QStringList PgrepArgs;
    PgrepArgs<<"-P"<<"";
    QByteArray current_pid=QString::number(pid).toLocal8Bit();
    do {
        PgrepArgs[1]=current_pid;
        Pgrep.start("/usr/bin/pgrep", PgrepArgs);
        if (Pgrep.waitForFinished()&&!Pgrep.exitCode()) {
            current_pid=Pgrep.readAllStandardOutput();
            current_pid.chop(1);
        } else
            break;
    } while (current_pid.indexOf('\n')==-1);

    Display *dpy=XOpenDisplay(NULL);
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    Window *MCompositor=NULL;

    //Get MCompositor window object
    int ret=XGetWindowProperty(dpy, XDefaultRootWindow(dpy), XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", False),
                               0, 0x7fffffff, False, XA_WINDOW, &actual_type, &actual_format, &nitems, &bytes_after,
                               (unsigned char **)&MCompositor);

    if(ret==Success) {
        QByteArray data(PgrepArgs[1].toLocal8Bit());
        data.append("\0\0", 2);                         //_MEEGO_SPLASH_SCREEN[0]=Pid (mandatory) & _MEEGO_SPLASH_SCREEN[1]=""
        data.append(splash.toLocal8Bit());
        data.append("\0", 1);                           //_MEEGO_SPLASH_SCREEN[2]=Portrait Splash (mandatory)
        data.append(splash_lscape.toLocal8Bit());
        data.append("\0\0", 2);                         //_MEEGO_SPLASH_SCREEN[3]=Landscape Splash & _MEEGO_SPLASH_SCREEN[4]=""

        //Set _MEEGO_SPLASH_SCREEN property for MCompositor
        XChangeProperty(dpy, *MCompositor, XInternAtom(dpy, "_MEEGO_SPLASH_SCREEN", False),
                        XA_STRING, 8, PropModeReplace,
                        (unsigned char *)data.constData(), data.size());
    }

    XCloseDisplay(dpy);

    return ret==Success;
}

const char* RunTools::DropNewLine(const char *in_buf, int buf_size)
{
    while (*in_buf++!='\n'&&--buf_size>0);
    return in_buf;
}

QString RunTools::QuotedJoin(const QStringList &list)
{
    QString cmd_line;
    bool single_item=true;
    /*foreach(QString cmd, list)
        cmd_line+=single_item?
                    single_item=false, cmd:
                    QString(" '%1'").arg(cmd.replace(QString("'"), QString("'\"'\"'")));*/
    foreach(QString cmd, list)
        if (single_item) {
            single_item=false;
            cmd_line+=cmd;
        } else
            cmd_line+=QString(" '%1'").arg(cmd.replace(QString("'"), QString("'\"'\"'")));
    return cmd_line;
}

bool RunTools::WaitForNoEcho(int &master_fd, bool immediate)
{
    termios tio;

    tcgetattr(master_fd, &tio);
    if (!immediate) {
        for (int i=0; (tio.c_lflag&ECHO)&&i<NO_ECHO_TRIES; i++) {
            usleep(1000);
            tcgetattr(master_fd, &tio);
        }
    }

    return !(tio.c_lflag&ECHO);
}

QString RunTools::GetProcName(pid_t pid)
{
    QFile CmdLineFile(QString("/proc/%1/cmdline").arg(pid));

    if (!CmdLineFile.open(QFile::ReadOnly|QFile::Text)) {
        return "";
    }

    return QString::fromLocal8Bit(CmdLineFile.readAll());
}

char** RunTools::StringListToArray(const QStringList &list)
{
    char** tmp=new char*[list.count()+1];
    char** arr=tmp;

    foreach (const QString &str, list) {
        *tmp=new char[str.toLocal8Bit().length()+1];
        strcpy(*tmp, str.toLocal8Bit().constData());
        tmp++;
    }
    *tmp=NULL;

    return arr;
}

void RunTools::DeleteStringArray(char** arr)
{
    char** tmp=arr;

    while (*tmp) {
        delete[] *tmp;
        tmp++;
    }

    delete[] arr;
}
