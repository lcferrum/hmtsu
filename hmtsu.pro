# HMTsu
# Copyright (C) 2013 Lcferrum
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# Harmattan boosting through applauncherd daemon requires qdeclarative-boostable
# MNotification and MGConfItem uses meegotouch
CONFIG += qdeclarative-boostable meegotouch

LIBS += -lcrypt -lutil -lX11

SOURCES +=  src/main.cpp \
            src/context.cpp \
            src/modevalidator.cpp \
            src/uservalidator.cpp \
            src/commhandler.cpp \
            src/runtools.cpp \
            src/contranslator.cpp \
            src/hout.cpp \
            src/iconprovider.cpp \
            src/pswtools.cpp

HEADERS +=  src/context.h \
            src/runmodes.h \
            src/modevalidator.h \
            src/uservalidator.h \
            src/commhandler.h \
            src/runtools.h \
            src/common.h \
            src/contranslator.h \
            src/hout.h \
            src/iconprovider.h \
            src/pswtools.h

# Things required for deployment and Harmattan boosting
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES +=  qtc_packaging/debian_harmattan/rules \
                qtc_packaging/debian_harmattan/manifest.aegis \
                qtc_packaging/debian_harmattan/copyright \
                qtc_packaging/debian_harmattan/control \
                qtc_packaging/debian_harmattan/compat \
                qtc_packaging/debian_harmattan/changelog \
                qtc_packaging/debian_harmattan/hmtsu.links

# Hack for lupdate
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
