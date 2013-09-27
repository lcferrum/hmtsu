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
import com.nokia.meego 1.0
import com.lcferrum.hmtsu 1.0   //created at runtime

Page {
    orientationLock: PageOrientation.LockPortrait

    property int propAtsRemain: MAX_PSW_ATTEMPTS
    property bool propNoPass: false

    function fnGetMessage() {
        if (!objContext.IfCustomMessage()) {
            if (objContext.TargetUser=="root") {
                return (objContext.Mode==RunModes.SUDO?qsTr("__default_sudo_root_msg%D__"):qsTr("__default_root_msg%D__")).replace("%D", objContext.GetText());
            } else {
                return (objContext.Mode==RunModes.SUDO?qsTr("__default_sudo_msg%U%D__"):qsTr("__default_msg%U%D__")).replace("%U", objContext.TargetUser).replace("%D", objContext.GetText());
            }
        } else {
            return objContext.GetText();
        }
    }

    function fnCheckPassword() {
        if (objContext.Mode==RunModes.PRINT) {
            objIntercom.SetCustomExitCode(NORMAL_EXIT_CODE);
            idBtnLaunch.enabled=false;
            idPassInput.platformCloseSoftwareInputPanel();
            idPassInput.readOnly=true;
            idRunTimer.start();
        } else if (propNoPass) {
            objIntercom.SetCustomExitCode(NORMAL_EXIT_CODE);
            idBtnLaunch.enabled=false;
            idRunTimer.start();
        } else {
            objPassCheck.PswCheck(idPassInput.text);
        }
    }

    Connections {
        target: objPassCheck
        onSignalPswOk: {
            objIntercom.SetCustomExitCode(NORMAL_EXIT_CODE);
            idBtnLaunch.enabled=false;
            idPassInput.platformCloseSoftwareInputPanel();
            idPassInput.readOnly=true;
            if (!objContext.ShowSplash())
                objIntercom.AddInfo(qsTr("__pass_ok__"));
            idRunTimer.start();
        }
        onSignalPswBad: {
            propAtsRemain--;
            idPassInput.errorHighlight=true;
            if (propAtsRemain>0) {
                objIntercom.AddInfo(qsTr("__pass_wrong_cnt%R__").replace("%R", propAtsRemain));
            } else {
                objIntercom.SetCustomExitCode(DENIED_EXIT_CODE);
                idBtnLaunch.enabled=false;
                idPassInput.platformCloseSoftwareInputPanel();
                idPassInput.readOnly=true;
                objIntercom.AddInfo(qsTr("__pass_wrong_end__"));
            }
        }
        onSignalNoPsw: {
            propNoPass=true;
            idPassInput.readOnly=true;
            idPassInput.placeholderText=qsTr("__pass_not_needed__");
        }
    }

    Component.onCompleted: {
        objIntercom.SetCustomExitCode(CANCELED_EXIT_CODE);
        objPassCheck.Prepare(objContext.Mode, objContext.TargetUser);
        if (objContext.GetIcon().length>0) {
            idBlancIcon.source="gfx/blanc.png";
            idProgramIcon.source=objContext.GetIcon();
        }
    }

    Timer {
        id: idRunTimer
        interval: 1000
        repeat: false
        onTriggered: {
            objContext.Run(idPassInput.text, propNoPass);
            Qt.quit();
        }
    }

    TopHeader {
        id: idBannerTop
        color: "#7DA4D3"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        text: qsTr("__enter_pass__")
    }

    Label {
        id: idLabelTop
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: idBannerTop.bottom
        anchors.topMargin: 16
        width: parent.width-32;

        Component.onCompleted: {
            text=fnGetMessage();
        }
    }

    TextField {
        id: idPassInput
        width: parent.width-64;
        echoMode: TextInput.Password
        inputMethodHints: Qt.ImhNoAutoUppercase|Qt.ImhNoPredictiveText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: idBtnLaunch.top
        anchors.bottomMargin: 16
        onTextChanged: {
            errorHighlight=false;
        }
        Keys.onReturnPressed: {
            fnCheckPassword();
        }
    }

    Button {
        id: idBtnLaunch
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 420
        text: qsTr("__launch__")
        onClicked: {
            fnCheckPassword();
        }
    }

    Item {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: idBtnLaunch.bottom
        height: 272

        Image {
            id: idBlancIcon
            source: "gfx/logo.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Image {
                id: idProgramIcon
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    tools: ToolBarLayout {
        visible: true
        ToolIcon {
            iconSource: "image://theme/icon-s-description"
            anchors.right: parent.right
            onClicked: idAboutDialog.open()
        }
    }
}
