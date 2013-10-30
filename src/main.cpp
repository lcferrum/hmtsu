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

#include <QtGui/QApplication>
#include <QLocale>
#include <QTranslator>
#include <QGraphicsObject>
#include <QtDeclarative>
#include <QDebug>
#include "context.h"
#include "runmodes.h"
#include "modevalidator.h"
#include "uservalidator.h"
#include "pswchecker.h"
#include "commhandler.h"
#include "common.h"
#include "contranslator.h"
#include "qmlapplicationviewer.h"

void SuppressQDebug(QtMsgType type, const char *msg)
{
     Q_UNUSED(type);
     Q_UNUSED(msg);
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> App(createApplication(argc, argv));

    QString locale=QLocale::system().name();
    QTranslator Translator;
    if (!(Translator.load("tr_"+locale, ":/i18n")))
        Translator.load("tr_en", ":/i18n");
    App->installTranslator(&Translator);

    ConTranslator ConsoleMsgs;
    ConsoleMsgs.load("conmsgs", ":/i18n");
    App->installTranslator(&ConsoleMsgs);

    Context Ctx(argc, argv, locale);

    if (Ctx.IfExit())
        return IntercomHandler::GetExitCode();

    if (Ctx.GetVerboseLevel()<1) IntercomHandler::DisableGeneralMsgs();      // GENERAL verbosity included in levels greater than 0
    if (Ctx.GetVerboseLevel()<2) IntercomHandler::DisableErrorMsgs();        // ERRORS verbosity included in levels greater than 1
    if (Ctx.GetVerboseLevel()<3) IntercomHandler::DisableWarningMsgs();      // WARNINGS verbosity included in levels greater than 2
    if (Ctx.GetVerboseLevel()<4) qInstallMsgHandler(SuppressQDebug);         // DEBUG verbosity included in levels greater than 3

    {
        ModeValidator ModesList(Ctx.CheckMode(), Ctx.CheckMode()==RunModes::PRINT);
        UserValidator UsersList(Ctx.GetTargetUser(), Ctx.CheckMode()==RunModes::PRINT);
        PswChecker PassCheck;
        ScopedIntercomHandler ViewerIntercomHandler;
        QmlApplicationViewer Viewer;

        qmlRegisterUncreatableType<RunModes>("com.lcferrum.hmtsu", 1, 0, "RunModes", "Exports RunModes enum to QML");
        Viewer.rootContext()->setContextProperty("objModesList", &ModesList);
        Viewer.rootContext()->setContextProperty("objUsersList", &UsersList);
        Viewer.rootContext()->setContextProperty("objContext", &Ctx);
        Viewer.rootContext()->setContextProperty("objIntercom", &ViewerIntercomHandler);
        Viewer.rootContext()->setContextProperty("objPassCheck", &PassCheck);
        Viewer.rootContext()->setContextProperty("HMTSU_VERSION_STRING", HMTSU_VERSION_STRING IF_DEBUG(" (DEBUG)"));
        Viewer.rootContext()->setContextProperty("HMTSU_COPYRIGHT_STRING", HMTSU_COPYRIGHT_STRING);
        Viewer.rootContext()->setContextProperty("CANCELED_EXIT_CODE", CANCELED_EXIT_CODE);
        Viewer.rootContext()->setContextProperty("DENIED_EXIT_CODE", DENIED_EXIT_CODE);
        Viewer.rootContext()->setContextProperty("NORMAL_EXIT_CODE", NORMAL_EXIT_CODE);
        Viewer.rootContext()->setContextProperty("MAX_PSW_ATTEMPTS", MAX_PSW_ATTEMPTS);

        Viewer.setSource(QUrl("qrc:/main.qml"));

        Viewer.showExpanded();

        App->exec();
    }

    Ctx.ActuallyRun();

    return IntercomHandler::GetExitCode();
}
