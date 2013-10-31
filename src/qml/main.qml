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

import QtQuick 1.1
import com.nokia.meego 1.1

PageStackWindow {
    showStatusBar: false
    showToolBar: true

    initialPage: objContext.IfContinue()?idPassPage:idStartPage

    Connections {
        target: objIntercom
        onSignalError: {
            idErrorDialog.message=msg;
            idErrorDialog.open();
        }
    }

    Component.onCompleted: {
        theme.colorScheme=8;
        idIntercomDelayedStartTimer.start();
    }

    Timer {
        id: idIntercomDelayedStartTimer
        interval: 100   //Obtained through trial and error method
        repeat: false
        onTriggered: {
            objIntercom.Start();
        }
    }

    Component {
        id: idPassPage
        PassPage {}
    }

    Component {
        id: idStartPage
        StartPage {}
    }

    Dialog {
        id: idAboutDialog

        title: Label {
            height: screen.currentOrientation===Screen.Portrait?UiConstants.HeaderDefaultHeightPortrait:UiConstants.HeaderDefaultHeightLandscape
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            text: "<b>HMTsu</b> <i><small>"+HMTSU_VERSION_STRING+"</small></i>"
            font.pixelSize: UiConstants.HeaderFont.pixelSize
            font.family: UiConstants.HeaderFont.family
        }

        content: Column {
            width: screen.currentOrientation===Screen.Portrait?screen.displayHeight:screen.displayWidth
            spacing: UiConstants.ButtonSpacing
            anchors.horizontalCenter: parent.horizontalCenter

            Image {
                source: "file:///usr/share/icons/hicolor/80x80/apps/hmtsu80.png"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                text: qsTr("__hmtsu_description__")
                horizontalAlignment: Text.AlignHCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: UiConstants.DefaultMargin*2
                anchors.rightMargin: UiConstants.DefaultMargin*2
                color: "white";
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
            }

            Label {
                text: "<br>© "+HMTSU_COPYRIGHT_STRING
                horizontalAlignment: Text.AlignHCenter
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: UiConstants.GroupHeaderFont.pixelSize
                font.bold: true
                color: "grey";
            }

            Label {
                text: "<tt>This program comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome to redistribute it under the terms of the GNU General Public License; see the source code for details.</tt>"
                horizontalAlignment: Text.AlignJustify
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: UiConstants.DefaultMargin
                anchors.rightMargin: UiConstants.DefaultMargin
                font.pixelSize: UiConstants.GroupHeaderFont.pixelSize
                color: "grey";
            }
        }
    }

    QueryDialog {
        id: idErrorDialog
        titleText: " "
        icon: "image://theme/icon-l-error"
        rejectButtonText: qsTr("__error_close__")
        onRejected: Qt.quit()
    }
}
