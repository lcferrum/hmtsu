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
import com.lcferrum.hmtsu 1.0   //Created at runtime

Page {
    property int propAtsRemainPri: MAX_PSW_ATTEMPTS
    property int propAtsRemainSec: MAX_PSW_ATTEMPTS

    function fnGetMessage() {
        if (!objContext.IfCustomMessage()) {
            if (objContext.TargetUser===objContext.GetRootName()) {
                return (objContext.Mode===RunModes.SUDO?qsTr("__default_sudo_root_msg%D__"):qsTr("__default_root_msg%D__")).replace("%D", objContext.GetText());
            } else {
                return (objContext.Mode===RunModes.SUDO?qsTr("__default_sudo_msg%U%D__"):(objContext.Mode===RunModes.ARIADNE?qsTr("__default_ariadne_msg%U%D__"):qsTr("__default_msg%U%D__"))).replace("%U", objContext.TargetUser).replace("%D", objContext.GetText());
            }
        } else {
            return objContext.GetText();
        }
    }

    function fnCheckPassword() {
        if (objContext.Mode===RunModes.PRINT) {
            objIntercom.SetCustomExitCode(NORMAL_EXIT_CODE);
            idBtnLaunch.enabled=false;
            idPassInput.platformCloseSoftwareInputPanel();
            idPassInput.readOnly=true;
            objContext.PrepareToRun(idPassInput.text, false);
            idRunTimer.start();
        } else {
            propAtsRemainPri--;
            if (propAtsRemainPri>=0) {
                objPassCheck.PswCheck(idPassInput.text);
            }
            if (propAtsRemainPri<=0) {
                idBtnLaunch.enabled=false;
                idPassInput.platformCloseSoftwareInputPanel();
                idPassInput.readOnly=true;
            }
        }
    }

    Connections {
        target: objPassCheck

        onSignalPswOk: {
            objIntercom.SetCustomExitCode(NORMAL_EXIT_CODE);
            idBtnLaunch.enabled=false;
            idPassInput.platformCloseSoftwareInputPanel();
            idPassInput.readOnly=true;
            objIntercom.AddInfo(qsTr("__pass_ok__"));
            objContext.PrepareToRun(psw, no_pass);
            idRunTimer.start();
        }

        onSignalPswBad: {
            propAtsRemainSec--;
            idPassInput.errorHighlight=true;
            if (propAtsRemainSec>0) {
                objIntercom.AddInfo(qsTr("__pass_wrong_cnt%R__").replace("%R", propAtsRemainSec));
            } else {
                objIntercom.SetCustomExitCode(DENIED_EXIT_CODE);
                objIntercom.AddInfo(qsTr("__pass_wrong_end__"));
            }
        }

        onSignalNoPsw: {
            idPassInput.readOnly=true;
            idPassInput.text="";
            idPassInput.placeholderText=qsTr("__pass_not_needed__");
        }
    }

    Component.onCompleted: {
        objIntercom.SetCustomExitCode(CANCELED_EXIT_CODE);
        objPassCheck.PrepareForCheck(objContext.Mode, objContext.TargetUser);
        idNoVkbFlickerTimer.start();
    }

    Timer {
        id: idRunTimer
        interval: 1000
        repeat: false

        onTriggered: Qt.quit()
    }

    Timer {
        id: idNoVkbFlickerTimer
        interval: 350   //Obtained through trial and error method
        repeat: false

        onTriggered: idPassInput.forceActiveFocus()
    }

    TopHeader {
        id: idBannerTop
        color: "#7DA4D3"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        landscape: screen.currentOrientation===Screen.Landscape
        text: qsTr("__enter_pass__")
    }

    Flickable {
        anchors.leftMargin: UiConstants.DefaultMargin
        anchors.rightMargin: UiConstants.DefaultMargin
        anchors.fill: parent                //TextField VKB auto-scroll fix
        z: idBannerTop.z-1                  //TextField VKB auto-scroll fix
        flickableDirection: Flickable.VerticalFlick
        contentHeight: idPassInputItem.height+UiConstants.DefaultMargin
        clip: true

        Column {
            id: idPassInputItem
            width: parent.width
            spacing: UiConstants.DefaultMargin/2

            Item {
                width: parent.width
                height: idBannerTop.height  //TextField VKB auto-scroll fix
            }

            Row {
                width: parent.width
                spacing: idAppIcon.width>0?UiConstants.DefaultMargin:0

                Image {
                    id: idAppIcon
                    anchors.verticalCenter: parent.verticalCenter
                    source: objContext.GetIcon()
                }

                Label {
                    width: parent.width-idAppIcon.width-parent.spacing
                    anchors.verticalCenter: parent.verticalCenter

                    Component.onCompleted: text=fnGetMessage()  //Prevents "non-NOTIFYable properties dependency" warning
                }
            }

            Column {
                width: parent.width
                spacing: UiConstants.DefaultMargin

                Image {
                    height: 2
                    width: parent.width
                    source: "image://theme/meegotouch-groupheader-background"
                }

                TextField {
                    id: idPassInput
                    width: screen.displayHeight-UiConstants.DefaultMargin*2
                    anchors.horizontalCenter: parent.horizontalCenter
                    echoMode: TextInput.Password
                    inputMethodHints: Qt.ImhNoAutoUppercase|Qt.ImhNoPredictiveText
                    platformSipAttributes: SipAttributes {
                        actionKeyLabel: qsTr("__launch__")
                        actionKeyHighlighted: true
                    }

                    Keys.onReturnPressed: fnCheckPassword()

                    onTextChanged: errorHighlight=false
                }

                Button {
                    id: idBtnLaunch
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("__launch__")

                    onClicked: fnCheckPassword()
                }
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
