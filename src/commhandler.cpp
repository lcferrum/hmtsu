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

#include <MNotification>
#include <iostream>
#include "commhandler.h"
#include "common.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#define qKeepCommas(...)    __VA_ARGS__

int IntercomHandler::ref_count=0;
Intercommunication *IntercomHandler::Intercom=NULL;

Intercommunication::Intercommunication():
    QObject(NULL),
    MessageQueue(), started(false), error_msgs_enabled(true), warning_msgs_enabled(true), general_msgs_enabled(true), err_msg(), error(false), exit_code(NORMAL_EXIT_CODE)
{
}

void Intercommunication::AddError(const QString &msg)
{
    if (!error) {
        if (started)
            signalError(QVariant(msg));
        else
            err_msg=msg;
        exit_code=ERROR_EXIT_CODE;
        error=true;
    }
}

void Intercommunication::AddSafeError(const QString &msg)
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

void Intercommunication::AddSafeWarning(const QString &msg)
{
    if (!error&&started&&warning_msgs_enabled)
        Notify(msg, cmm_ntf_WARNING);
}

void Intercommunication::AddWarning(const QString &msg)
{
    if (!error&&warning_msgs_enabled) {
        if (started)
            Notify(msg, cmm_ntf_WARNING);
        else
            MessageQueue.append(qMakePair(msg, cmm_ntf_WARNING));
    }
}

void Intercommunication::AddInfo(const QString &msg)
{
    if (!error&&general_msgs_enabled) {
        if (started)
            Notify(msg, cmm_ntf_INFO);
        else
            MessageQueue.append(qMakePair(msg, cmm_ntf_INFO));
    }
}

void Intercommunication::Notify(const QString &msg, cmm_ntfs type)
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
        started=true;
        if (error) {
            signalError(QVariant(err_msg));
        } else {
            foreach(qKeepCommas(const QPair<QString, cmm_ntfs> &msg), MessageQueue)
                Notify(msg.first, msg.second);
        }
    }
}

void Intercommunication::ToggleErrorMsgs(bool state)
{
    error_msgs_enabled=state;
}

void Intercommunication::ToggleWarningMsgs(bool state)
{
    warning_msgs_enabled=state;
}

void Intercommunication::ToggleGeneralMsgs(bool state)
{
    general_msgs_enabled=state;
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
