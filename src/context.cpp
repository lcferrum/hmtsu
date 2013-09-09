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

#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>
#include <iostream>
#include <QSettings>
#include <QCoreApplication>
#include <QTextCodec>
#include "context.h"
#include "common.h"

using namespace std;

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
    {"command-file",    0, NULL, 'f'},
    {"verbosity-level", 1, NULL, 'V'},
    {NULL, 0, NULL, 0}
};

void PrintUsage();
void PrintVersion();

Context::Context(int argc, char **argv, QString lang):
    QObject(NULL)
{
    run_mode=RunModes::SU;
    Tools=NULL;
    action=ctx_act_ASK_FOR_MORE;
    message=ctx_msg_NULL;
    verbosity=CND_DEBUG(4,3);
    login=false;
    kpp_env=false;
    user="root";
    text="";

    int opt=0;
    bool command_file=false;

    while ((opt=getopt_long(argc, argv, "?hvu:lpm:kSwaD:V:", long_options, NULL))!=-1) {    //temporary removed -f option
    //while ((opt=getopt_long(argc, argv, "?hvu:lpm:kSwaD:fV:", long_options, NULL))!=-1) {
        switch (opt) {
            case 0:
                break;
            case 'h':
                PrintUsage();
                action=ctx_act_EXIT;
                return;
            case 'v':
                PrintVersion();
                action=ctx_act_EXIT;
                return;
            case '?':
                PrintUsage();
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
                    if (LoadTextFromDesktop(optarg, "Comment", lang)) message=ctx_msg_FULL;
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
                    if (LoadTextFromDesktop(optarg, "Name", lang)) message=ctx_msg_DESC;
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
                command_file=true;
                break;
            case 'V':
                {
                    bool intok;
                    verbosity=QString::fromLocal8Bit(optarg).toInt(&intok, 10);
                    if (!intok||verbosity<0||verbosity>4) {
                        Intercom->AddError(QCoreApplication::translate("Messages", "__context_verbosity_err__"));
                        return;
                    }
                }
                break;
        }
    }

    if (run_mode==RunModes::PRINT) {
        if (message==ctx_msg_NULL) {
            Intercom->AddError(QCoreApplication::translate("Messages", "__context_printpass_err__"));
            action=ctx_act_ASK_FOR_MORE;
        } else {
            action=ctx_act_CONTINUE;
        }
        return;
    }

    if (argc<=optind) {
        if (command_file)
            Intercom->AddError(QCoreApplication::translate("Messages", "__context_commandfile_err__"));
        action=ctx_act_ASK_FOR_MORE;
        return;
    } else {
        if (command_file) {
            if (!access(argv[optind], R_OK)&&LoadCommandFromDesktop(argv[optind])) {
                if (message==ctx_msg_NULL) message=ctx_msg_CMD;
                action=ctx_act_CONTINUE;
            } else {
                Intercom->AddWarning(QCoreApplication::translate("Messages", "__context_commandfile_wrn%1__").arg(argv[optind]));
                action=ctx_act_ASK_FOR_MORE;
            }
            return;
        } else {
            while (optind<argc)
                command<<QString::fromLocal8Bit(argv[optind++]);

            if (message==ctx_msg_NULL) message=ctx_msg_CMD;
            action=ctx_act_CONTINUE;
            return;
        }
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

void Context::SetTargetUser(QString str)
{
    user=str;
}

void Context::SetCommand(QString str)
{
    ToArgv(str);
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

bool Context::LoadTextFromDesktop(QString fname, QString key, QString lang)
{
    QSettings desktop(fname, QSettings::IniFormat);
    desktop.setIniCodec(QTextCodec::codecForName("UTF-8"));
    desktop.beginGroup("Desktop Entry");

    QVariant val=desktop.value(QString("%1[%2]").arg(key, lang));

    if (val.isValid()) {
        text=val.toString();
        return true;
    } else if ((val=desktop.value(QString("%1[%2]").arg(key, lang.left(2)))).isValid()) {
        text=val.toString();
        return true;
    } else if ((val=desktop.value(key)).isValid()) {
        text=val.toString();
        return true;
    } else {
        Intercom->AddWarning(QCoreApplication::translate("Messages", "__context_descfile_wrn%1%2__").arg(key, fname));
        return false;
    }
}

bool Context::LoadCommandFromDesktop(QString fname)
{
    QSettings desktop(fname, QSettings::IniFormat);
    desktop.setIniCodec(QTextCodec::codecForName("UTF-8"));
    desktop.beginGroup("Desktop Entry");

    QVariant val=desktop.value(QString("Exec"));

    if (val.isValid()) {
        return ToArgv(val.toString());
    } else {
        return false;
    }
}

bool Context::ToArgv(QString cmdline)
{
    wordexp_t args;

    if(!wordexp(cmdline.toUtf8().constData(), &args, WRDE_UNDEF)) {
        for (unsigned int i=0; i<args.we_wordc; i++)
            command<<QString::fromUtf8(args.we_wordv[i]);

        wordfree(&args);
        return true;
    } else {
        command.clear();
        command<<cmdline;

        Intercom->AddWarning(QCoreApplication::translate("Messages", "__context_wordexp_wrn__"));
        return false;
    }
}

int Context::GetVerboseLevel()
{
    return verbosity;
}

void Context::Run(QString psw, bool no_pass)
{
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
    psw.fill('\0');
    psw.clear();
}

void Context::ActuallyRun()
{
    if (Tools) {
        Tools->Run(user, login, kpp_env, command);
        delete Tools;
    }
}

void PrintUsage() {
    cout<<
"Usage: hmtsu [-u <user>] [options] <command>"
"\n"
    <<endl;
    cout<<
"  --help, -h\n"
"    Display this help and exit."
    <<endl;
    cout<<
"  --version, -v\n"
"    Output version information and exit."
    <<endl;
    cout<<
"  --verbosity-level, -V\n"
"    This option controls pop-up's verbosity and\n"
"    debug output availability. Each level includes\n"
"    the previous levels as well.\n"
"    LEVEL   DESCRIPTION\n"
"    -----   ----------------------------------------\n"
"        0 - No pop-ups, only error dialogs and\n"
"            standart/error output.\n"
"        1 - General informational pop-up messages.\n"
"        2 - Error pop-up messages.\n"
"        3 - Warning pop-up messages"IFNOT_DEBUG(" (default level)")".\n"
"        4 - Debug output"IF_DEBUG(" (default level)")"."
"\n"
    <<endl;
    cout<<
"  --user <user>, -u <user>\n"
"    Call <command> as the specified user. By default\n"
"    it is root."
"\n"
    <<endl;
    cout<<
"  --preserve-env, -k\n"
"    Try to preserve the current environments."
    <<endl;
    cout<<
"  --login, -l\n"
"    Make this a login shell."
"\n"
    <<endl;
    cout<<
"  --description <description|file>, -D <description|file>\n"
"    Provide a descriptive name for the command to\n"
"    be used in the default message, making it nicer.\n"
"    You can also provide the absolute path for a\n"
"    .desktop file. The Name key will be used in this\n"
"    case."
    <<endl;
    cout<<
"  --message <message|file>, -m <message|file>\n"
"    Replace the standard message shown to ask for\n"
"    password for the argument passed to the option.\n"
"    Only use this if --description does not suffice.\n"
"    You can also provide the absolute path for a\n"
"    .desktop file. The Comment key will be used in\n"
"    this case."
"\n"
    <<endl;
/*    cout<<
"  --command-file, -f\n"
"    Treat command argument as path to .desktop file\n"
"    containing actual command in Exec key."
"\n"
    <<endl;*/
    cout<<
"  --print-pass, -p\n"
"    Ask HMTsu to print the password to stdout, just\n"
"    like ssh-askpass. Useful to use in scripts with\n"
"    programs that accept receiving the password on\n"
"    stdin. HMTsu doesn't check password and user\n"
"    existence during this."
"\n"
    <<endl;
    cout<<
"  --sudo-mode, -S\n"
"    Make HMTsu use sudo."
    <<endl;
    cout<<
"  --su-mode, -w\n"
"    Make HMTsu use devel-su (default mode)."
    <<endl;
    cout<<
"  --ariadne-mode, -a\n"
"    Make HMTsu use ariadne."
"\n"
    <<endl;
    cout<<
"  --\n"
"    The -- option indicates that HMTsu should stop\n"
"    processing command line arguments."
    <<endl;
}

void PrintVersion() {
    cout<<
"HMTsu "<<HMTSU_VERSION_STRING<<IF_DEBUG(" (DEBUG)")"\n"
"\n"
"Copyright (C) 2013 "<<HMTSU_COPYRIGHT_STRING<<"\n"
"This program comes with ABSOLUTELY NO WARRANTY.\n"
"This is free software, and you are welcome to redistribute it\n"
"under the terms of the GNU General Public License;\n"
"see the source code for details.\n"
"\n"
"Try '-h' option for more information.\n"
    <<endl;
}
