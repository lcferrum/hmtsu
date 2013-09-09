#include <MNotification>
#include <iostream>
#include <unistd.h>
#include "commhandler.h"
#include "common.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#define qKeepCommas(...)    __VA_ARGS__

int IntercomHandler::ref_count=0;
Intercommunication *IntercomHandler::Intercom=NULL;

Intercommunication::Intercommunication():
    QObject(NULL)
{
    started=false;
    error=false;
    exit_code=NORMAL_EXIT_CODE;
    error_msgs_enabled=true;
    warning_msgs_enabled=true;
    general_msgs_enabled=true;
    istty=ttyname(STDOUT_FILENO)&&ttyname(STDERR_FILENO)&&ttyname(STDIN_FILENO);    //STDIN && STDERR also enabled when launching with invoker
}

void Intercommunication::AddError(QString msg)
{
    if (!error) {
        if (started) {
            signalError(QVariant(msg));
        } else {
            err_msg=msg;
        }
        exit_code=ERROR_EXIT_CODE;
        error=true;
    }
}

void Intercommunication::AddSafeError(QString msg)
{
    if (!error&&started) {
        if (error_msgs_enabled)
            Notify(msg, cmm_ntf_SERROR);
        else
            std::cerr<<qPrintable(msg)<<std::endl;
        exit_code=ERROR_EXIT_CODE;
        error=true;
    }
}

void Intercommunication::AddSafeWarning(QString msg)
{
    if (!error&&started&&warning_msgs_enabled)
        Notify(msg, cmm_ntf_WARNING);
}

void Intercommunication::AddWarning(QString msg)
{
    if (!error&&warning_msgs_enabled) {
        if (started) {
            Notify(msg, cmm_ntf_WARNING);
        } else {
            MessageQueue.append(qMakePair(msg, cmm_ntf_WARNING));
        }
    }
}

void Intercommunication::AddInfo(QString msg)
{
    if (!error&&general_msgs_enabled) {
        if (started) {
            Notify(msg, cmm_ntf_INFO);
        } else {
            MessageQueue.append(qMakePair(msg, cmm_ntf_INFO));
        }
    }
}

void Intercommunication::Notify(QString msg, cmm_ntfs type)
{
    MNotification Ntf(MNotification::DeviceEvent, "", msg);

    IF_DEBUG(qDebug()<<"Notification published:"<<msg;)

    switch (type) {
        case cmm_ntf_WARNING:
            Ntf.setImage("icon-m-transfer-error");
            break;
        case cmm_ntf_INFO:
            Ntf.setImage("icon-m-content-description-inverse");
            break;
        case cmm_ntf_SERROR:
            Ntf.setImage("icon-l-error");
            break;
    }

    Ntf.publish();
}

bool Intercommunication::IfError()
{
    return error;
}

void Intercommunication::Start()
{
    if (!started) {
        if (error) {
            signalError(QVariant(err_msg));
        } else {
            foreach(qKeepCommas(const QPair<QString, cmm_ntfs> &msg), MessageQueue)
                Notify(msg.first, msg.second);
            started=true;
        }
    }
}

void Intercommunication::DisableErrorMsgs()
{
    error_msgs_enabled=false;
}

void Intercommunication::DisableWarningMsgs()
{
    warning_msgs_enabled=false;
}

void Intercommunication::DisableGeneralMsgs()
{
    general_msgs_enabled=false;
}

bool Intercommunication::IfErrorMsgsDisabled()
{
    return !error_msgs_enabled;
}

int Intercommunication::GetExitCode()
{
    return exit_code;
}

void Intercommunication::SetCustomExitCode(int code)
{
    if (!error) exit_code=code;
}

IntercomHandler::IntercomHandler()
{
    if (!ref_count++) Intercom=new Intercommunication();
}

IntercomHandler::~IntercomHandler()
{
    if (!--ref_count) delete Intercom;
}

int IntercomHandler::GetExitCode()
{
    if (Intercom) return Intercom->GetExitCode();
        else return 0;
}

void IntercomHandler::DisableErrorMsgs()
{
    if (Intercom) Intercom->DisableErrorMsgs();
}

void IntercomHandler::DisableWarningMsgs()
{
    if (Intercom) Intercom->DisableWarningMsgs();
}

void IntercomHandler::DisableGeneralMsgs()
{
    if (Intercom) Intercom->DisableGeneralMsgs();
}
