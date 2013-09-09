# Add more folders to ship with the application, here
#folder_01.source = src/qml
#folder_01.target = .
#DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

#symbian:TARGET.UID3 = 0xE65FD930

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
#symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=
# QT += dbus

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
# MNotification uses meegotouch
CONFIG += qdeclarative-boostable meegotouch

LIBS += -lcrypt -lutil

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES +=  src/main.cpp \
            src/context.cpp \
            src/modevalidator.cpp \
            src/uservalidator.cpp \
            src/commhandler.cpp \
            src/pswchecker.cpp \
            src/runtools.cpp \
            src/contranslator.cpp

HEADERS +=  src/context.h \
            src/runmodes.h \
            src/modevalidator.h \
            src/uservalidator.h \
            src/commhandler.h \
            src/pswchecker.h \
            src/runtools.h \
            src/common.h \
            src/contranslator.h

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES +=  qtc_packaging/debian_harmattan/rules \
                qtc_packaging/debian_harmattan/manifest.aegis \
                qtc_packaging/debian_harmattan/copyright \
                qtc_packaging/debian_harmattan/control \
                qtc_packaging/debian_harmattan/compat \
                qtc_packaging/debian_harmattan/postinst \
                qtc_packaging/debian_harmattan/prerm \
                qtc_packaging/debian_harmattan/changelog

#Hack for lupdate
translate_qml {
    SOURCES +=  src/qml/PassPage.qml \
                src/qml/main.qml \
                src/qml/StartPage.qml \
                src/qml/TopHeader.qml \
                src/qml/GroupHeader.qml \
                src/qml/OptionsItem.qml \
                src/qml/OptionsSwitch.qml
}

TRANSLATIONS =  data/i18n/tr_en.ts \
                data/i18n/tr_ru.ts \
                data/i18n/conmsgs.ts

CODECFORSRC = UTF-8

RESOURCES +=    data/res.qrc \
                src/qml/qml.qrc
