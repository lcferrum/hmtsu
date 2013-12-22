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

#include <sys/ioctl.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>
#include <pwd.h>
#include <iostream>
#include <QCoreApplication>
#include <QScopedPointer>
#include <QRegExp>
#include <QStringList>
#include <QProcess>
#include <QFileInfo>
#include "iconprovider.h"
#include "context.h"
#include "common.h"
#include "pswtools.h"
#include "hout.h"

struct option long_options[] = {
    {"help",            0, NULL, 'h'},
    {"version",         0, NULL, 'v'},
    {"login",           0, NULL, 'l'},
    {"preserve-env",    0, NULL, 'k'},
    {"user",            1, NULL, 'u'},
    {"print-pass",      0, NULL, 'p'},
    {"message",         1, NULL, 'm'},
    {"sudo-mode",       0, NULL, 'S'},
    {"ariadne-mode",    0, NULL, 'a'},
    {"su-mode",         0, NULL, 'w'},
    {"description",     1, NULL, 'D'},
    {"force-desktop",   0, NULL, 'f'},
    {"verbosity-level", 1, NULL, 'V'},
    {NULL, 0, NULL, 0}
};

void SuppressQDebug(QtMsgType type, const char *msg)
{
     Q_UNUSED(type);
     Q_UNUSED(msg);
}

void PrintUsage(const QString &exe, const QString &su);
void PrintVersion();

Context::Context(int argc, char **argv):
    QObject(NULL),
    Tools(NULL), action(ctx_act_ASK_FOR_MORE), message(ctx_msg_NULL), verbosity(CND_DEBUG(4,3)), login(false), kpp_env(false), user(GetRootName()), text(), splash(), splash_lscape()
{
    QFileInfo ExePath(argv[0]);
    if (ExePath.fileName()=="hmtsudo")
        run_mode=RunModes::SUDO;
    else if (ExePath.fileName()=="hmtadn")
        run_mode=RunModes::ARIADNE;
    else
        run_mode=RunModes::SU;

    int opt=0;
    bool use_desktop_file=false;
    QScopedPointer<MDesktopEntry> CurDesktopFile(NULL);

    winsize argp;
    if (!ioctl(STDOUT_FILENO, TIOCGWINSZ, &argp)) {
        Hout::SetTerminalSize(argp.ws_col);
    }

    ApplyVerboseLevel();

    while ((opt=getopt_long(argc, argv, "?hvu:lpm:kSwaD:fV:", long_options, NULL))!=-1) {
        switch (opt) {
            case 0:
                break;
            case 'h':
                PrintUsage(ExePath.fileName(), GetRootName());
                action=ctx_act_EXIT;
                return;
            case 'v':
                PrintVersion();
                action=ctx_act_EXIT;
                return;
            case '?':
                PrintUsage(ExePath.fileName(), GetRootName());
                action=ctx_act_EXIT;
                return;
            case 'u':
                user=QString::fromLocal8Bit(optarg);
                break;
            case 'l':
                login=true;
                break;
            case 'p':
                run_mode=RunModes::PRINT;
                break;
            case 'm':
                if (!access(optarg, R_OK)) {
                    CurDesktopFile.reset(new MDesktopEntry(optarg));
                    if (LoadCommentFromDesktop(CurDesktopFile.data(), text)) message=ctx_msg_FULL;
                } else {
                    text=QString::fromLocal8Bit(optarg);
                    message=ctx_msg_FULL;
                }
                break;
            case 'k':
                kpp_env=true;
                break;
            case 'D':
                if (!access(optarg, R_OK)) {
                    CurDesktopFile.reset(new MDesktopEntry(optarg));
                    if (LoadNameFromDesktop(CurDesktopFile.data(), text)) message=ctx_msg_DESC;
                } else {
                    text=QString::fromLocal8Bit(optarg);
                    message=ctx_msg_DESC;
                }
                break;
            case 'S':
                if (run_mode!=RunModes::PRINT) run_mode=RunModes::SUDO;
                break;
            case 'w':
                if (run_mode!=RunModes::PRINT) run_mode=RunModes::SU;
                break;
            case 'a':
                if (run_mode!=RunModes::PRINT) run_mode=RunModes::ARIADNE;
                break;
            case 'f':
                use_desktop_file=true;
                break;
            case 'V':
                {
                    bool intok;
                    verbosity=QString::fromLocal8Bit(optarg).toInt(&intok, 10);
                    if (!intok||verbosity<0||verbosity>4) {
                        action=ctx_act_ASK_FOR_MORE;
                        Intercom->AddError(QCoreApplication::translate("Messages", "__context_verbosity_err__"));
                        return;
                    }
                    ApplyVerboseLevel();
                }
                break;
        }
    }

    if (use_desktop_file) {
        if (CurDesktopFile) {
            QString cmdline;
            LoadIconFromDesktop(CurDesktopFile.data(), icon);
            if (LoadExecFromDesktop(CurDesktopFile.data(), cmdline, splash, splash_lscape)) SetCommand(cmdline);   //Sets appropriate action (ctx_act_CONTINUE/ctx_act_ASK_FOR_MORE) internally
        } else {
            action=ctx_act_ASK_FOR_MORE;
            Intercom->AddError(QCoreApplication::translate("Messages", "__context_nodesktop_err__"));
            return;
        }
    }

    if (run_mode==RunModes::PRINT) {
        if (message==ctx_msg_FULL||message==ctx_msg_DESC) {
            action=ctx_act_CONTINUE;
        } else {
            action=ctx_act_ASK_FOR_MORE;
            Intercom->AddError(QCoreApplication::translate("Messages", "__context_printpass_err__"));
        }
        return;
    }

    if (argc>optind) {
        command.clear();

        while (optind<argc)
            command<<QString::fromLocal8Bit(argv[optind++]);

        if (message==ctx_msg_NULL) message=ctx_msg_CMD;
        action=ctx_act_CONTINUE;
    }
}

RunModes::QmlEnum Context::CheckMode()
{
    return run_mode;
}

void Context::ChangeMode(RunModes::QmlEnum mode)
{
    run_mode=mode;
}

bool Context::GetLogin()
{
    return login;
}

bool Context::GetPreserveEnv()
{
    return kpp_env;
}

bool Context::IfExit()
{
    return action==ctx_act_EXIT;
}

bool Context::IfCustomMessage()
{
    return message==ctx_msg_FULL;
}

bool Context::IfContinue()
{
    return action==ctx_act_CONTINUE;
}

QString Context::GetTargetUser()
{
    return user;
}

QString Context::GetText()
{
    return message==ctx_msg_CMD?command.join(" "):text;
}

void Context::SetTargetUser(const QString &user)
{
    this->user=user;
}

void Context::SetCommand(const QString &cmdline)
{
    wordexp_t args;

    command.clear();
    if(!wordexp(cmdline.toLocal8Bit().constData(), &args, WRDE_UNDEF)) {
        for (unsigned int i=0; i<args.we_wordc; i++)
            command<<QString::fromLocal8Bit(args.we_wordv[i]);

        wordfree(&args);
    } else {
        command<<cmdline;

        Intercom->AddWarning(QCoreApplication::translate("Messages", "__context_wordexp_wrn__"));
    }

    if (message==ctx_msg_NULL) message=ctx_msg_CMD;
    action=ctx_act_CONTINUE;
}

void Context::SetLogin(bool flag)
{
    login=flag;
}

void Context::SetPreserveEnv(bool flag)
{
    kpp_env=flag;
}

bool Context::LoadNameFromDesktop(const MDesktopEntry *CurDesktopFile, QString &value)
{
    if (CurDesktopFile&&CurDesktopFile->isValid()) {
        value=CurDesktopFile->name();
        return true;
    } else {
        Intercom->AddWarning(QCoreApplication::translate("Messages", "__context_descfile_wrn%1%2__").arg("Name", CurDesktopFile->fileName()));
        return false;
    }
}

bool Context::LoadCommentFromDesktop(const MDesktopEntry *CurDesktopFile, QString &value)
{
    if (CurDesktopFile&&CurDesktopFile->isValid()) {
        value=CurDesktopFile->comment();
        return true;
    } else {
        Intercom->AddWarning(QCoreApplication::translate("Messages", "__context_descfile_wrn%1%2__").arg("Comment", CurDesktopFile->fileName()));
        return false;
    }
}

bool Context::LoadIconFromDesktop(const MDesktopEntry *CurDesktopFile, QString &value)
{
    if (CurDesktopFile&&CurDesktopFile->isValid()) {
        value=CurDesktopFile->icon();
        return true;
    } else {
        Intercom->AddWarning(QCoreApplication::translate("Messages", "__context_descfile_wrn%1%2__").arg("Icon", CurDesktopFile->fileName()));
        return false;
    }
}

bool Context::LoadExecFromDesktop(const MDesktopEntry *CurDesktopFile, QString &cmdline, QString &S, QString &L)
{
    if (CurDesktopFile&&CurDesktopFile->isValid()) {
        QString exec=CurDesktopFile->exec();

        exec.replace(QRegExp("([^%])%[A-Za-z]"), "\\1");

        if (exec.startsWith("invoker")||exec.startsWith("/usr/bin/invoker")) {
            QRegExp SplashRx(" (--splash[ =]|-S ?)(.+) ");
            SplashRx.setMinimal(true);
            if (SplashRx.indexIn(exec)!=-1)
                S=SplashRx.cap(2);
            else
                S="";

            QRegExp SplashLscapeRx(" (--splash-landscape[ =]|-L ?)(.+) ");
            SplashLscapeRx.setMinimal(true);
            if (SplashLscapeRx.indexIn(exec)!=-1)
                L=SplashLscapeRx.cap(2);
            else
                L="";

            bool cmd_found=false;

            //Quick and dirty way to guess command line from invoker string without parsing it:
            //  Which util searches for valid executable in the string
            //  If found - everything starting from this executable is assumed to be command line
            foreach (const QString &token, exec.split(" ", QString::SkipEmptyParts).filter(QRegExp("^[^-\"']")).mid(1)) {
                QProcess which;
                which.start("/usr/bin/which", QStringList(token));
                if (which.waitForFinished()) {
                    if (!which.exitCode()) {
                        cmdline=exec.mid(exec.indexOf(token));
                        cmd_found=true;
                        break;
                    }
                }
            }

            if (!cmd_found) {
                Intercom->AddWarning(QCoreApplication::translate("Messages", "__context_badinvoke_wrn__"));
                cmdline=exec;
            }
        } else {
            cmdline=exec;
            S="";
            L="";
        }

        return true;
    } else {
        Intercom->AddWarning(QCoreApplication::translate("Messages", "__context_descfile_wrn%1%2__").arg("Exec", CurDesktopFile->fileName()));
        return false;
    }
}

QString Context::ForceDesktop(const QString &path)
{
    MDesktopEntry GuiApp(path);
    QString cmdline;
    if (LoadNameFromDesktop(&GuiApp, text)) message=ctx_msg_DESC;
        else message=ctx_msg_CMD;
    if (!LoadIconFromDesktop(&GuiApp, icon))
        icon="";
    if (!LoadExecFromDesktop(&GuiApp, cmdline, splash, splash_lscape)) {
        splash="";
        splash_lscape="";
    }
    return cmdline;
}

void Context::ApplyVerboseLevel()
{
    if (verbosity>0) Intercom->ToggleGeneralMsgs(true); //GENERAL verbosity included in levels greater than 0
        else Intercom->ToggleGeneralMsgs(false);
    if (verbosity>1) Intercom->ToggleErrorMsgs(true);   //ERRORS verbosity included in levels greater than 1
        else Intercom->ToggleErrorMsgs(false);
    if (verbosity>2) Intercom->ToggleWarningMsgs(true); //WARNINGS verbosity included in levels greater than 2
        else Intercom->ToggleWarningMsgs(false);
    if (verbosity>3) qInstallMsgHandler(0);             //DEBUG verbosity included in levels greater than 3
        else qInstallMsgHandler(SuppressQDebug);
}

QString Context::GetIcon()
{
    return IconProvider::ConvertPath(icon);
}

QString Context::GetRootName()
{
    passwd *user_record;

    user_record=getpwuid(ROOT_UID);

    if (user_record) {
        return QString::fromLocal8Bit(user_record->pw_name);
    } else {
        return "root";
    }
}

void Context::PrepareToRun(const QString &psw, bool no_pass)
{
    delete Tools;
    switch (run_mode) {
        case RunModes::PRINT:
            Tools=new PrintRunTools(psw, no_pass);
            break;
        case RunModes::SU:
            Tools=new SuRunTools(psw, no_pass);
            break;
        case RunModes::SUDO:
            Tools=new SudoRunTools(psw, no_pass);
            break;
        case RunModes::ARIADNE:
            Tools=new AriadneRunTools(psw, no_pass);
            break;
    }
}

void Context::Run()
{
    if (Tools) {
        Tools->Run(user, login, kpp_env, command, splash, splash_lscape);
        delete Tools;
        Tools=NULL;
    }
}

void PrintUsage(const QString &exe, const QString &su)
{
    Hout::Separator(qPrintable(QString("Usage: %1 [-u <user>] [options] [<command>]").arg(exe)));
    Hout::EmptyLine();
    Hout::Separator("--help, -h", 2);
    Hout::Paragraph("Display this help and exit.",
                    4);
    Hout::Separator("--version, -v", 2);
    Hout::Paragraph("Output version information and exit.",
                    4);
    Hout::Separator("--verbosity-level <level>, -V <level>", 2);
    Hout::Paragraph("This option controls pop-up's verbosity and debug output availability. Each level includes the previous levels as well.",
                    4);
    Hout::Separator("LEVEL   DESCRIPTION", 4);
    Hout::Separator("-----   ", 4, '-');
    Hout::Paragraph("No pop-ups, only error dialogs and standard/error output.",
                    12, "0 - ");
    Hout::Paragraph("General informational pop-up messages.",
                    12, "1 - ");
    Hout::Paragraph("Error pop-up messages.",
                    12, "2 - ");
    Hout::Paragraph("Warning pop-up messages" IFNOT_DEBUG(" (default level)") ".",
                    12, "3 - ");
    Hout::Paragraph("Debug output" IF_DEBUG(" (default level)") ".",
                    12, "4 - ");
    Hout::EmptyLine();
    Hout::Separator("--user <user>, -u <user>", 2);
    Hout::Paragraph(qPrintable(QString("Call <command> as the specified user (default: %1).").arg(su)),
                    4);
    Hout::EmptyLine();
    Hout::Separator("--preserve-env, -k", 2);
    Hout::Paragraph("Try to preserve the current environments.",
                    4);
    Hout::Separator("--login, -l", 2);
    Hout::Paragraph("Make this a login shell.",
                    4);
    Hout::EmptyLine();
    Hout::Separator("--description <description|file>, -D <description|file>", 2);
    Hout::Paragraph("Provide a descriptive name for the command to be used in the default message, making it nicer. You can also provide the absolute path for a .desktop file. The Name key will be used in this case.",
                    4);
    Hout::Separator("--message <message|file>, -m <message|file>", 2);
    Hout::Paragraph("Replace the standard message shown to ask for password for the argument passed to the option. Only use this if --description does not suffice. You can also provide the absolute path for a .desktop file. The Comment key will be used in this case.",
                    4);
    Hout::Separator("--force-desktop, -f", 2);
    Hout::Paragraph("Extract command line, application icon and splash image from .desktop file provided in --description or --message.",
                    4);
    Hout::EmptyLine();
    Hout::Separator("--print-pass, -p", 2);
    Hout::Paragraph("Ask HMTsu to print the password to stdout, just like ssh-askpass. Useful to use in scripts with programs that accept receiving the password on stdin. HMTsu doesn't check password and user existence during this.",
                    4);
    Hout::EmptyLine();
    Hout::Separator("--sudo-mode, -S", 2);
    Hout::Paragraph(qPrintable(QString("Make HMTsu use sudo%1.").arg(exe=="hmtsudo"?" (default mode)":"")),
                    4);
    Hout::Separator("--su-mode, -w", 2);
    Hout::Paragraph(qPrintable(QString("Make HMTsu use devel-su%1.").arg(exe!="hmtsudo"&&exe!="hmtadn"?" (default mode)":"")),
                    4);
    Hout::Separator("--ariadne-mode, -a", 2);
    Hout::Paragraph(qPrintable(QString("Make HMTsu use ariadne%1.").arg(exe=="hmtadn"?" (default mode)":"")),
                    4);
    Hout::EmptyLine();
    Hout::Separator("--", 2);
    Hout::Paragraph("The -- option indicates that HMTsu should stop processing command line arguments.",
                    4);
}

void PrintVersion()
{
    Hout::Separator("HMTsu " HMTSU_VERSION_STRING IF_DEBUG(" (DEBUG)"));
    Hout::EmptyLine();
    Hout::Separator("Copyright (C) " HMTSU_COPYRIGHT_STRING);
    Hout::Paragraph("This program comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome to redistribute it under the terms of the GNU General Public License; see the source code for details.");
    Hout::EmptyLine();
    Hout::Separator("Try '-h' option for more information.");
}
