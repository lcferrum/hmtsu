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
import com.lcferrum.hmtsu 1.0   //created at runtime

Page {
    //orientationLock: PageOrientation.LockPortrait

    property int propAtsRemain: MAX_PSW_ATTEMPTS
    property bool propNoPass: false

    function fnCurrentScreenHeight() {
        return screen.currentOrientation===Screen.Portrait?screen.displayWidth:screen.displayHeight;
    }

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
            objIntercom.SetCustomInfoIcon(objContext.GetIcon());
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
        landscape: screen.currentOrientation===Screen.Landscape
        text: qsTr("__enter_pass__")
    }

    Item {
        id: idScreenAnchor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: fnCurrentScreenHeight()
    }

    Item {
        id: idPassInputItem
        anchors.top: idBannerTop.bottom
        height: 80
        width: parent.width

        TextField {
            id: idPassInput
            width: Math.floor((screen.currentOrientation===Screen.Landscape?screen.displayWidth:screen.displayHeight)*0.87)
            //horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            //anchors.bottom: parent.height<fnCurrentScreenHeight()/2?parent.bottom:idScreenAnchor.verticalCenter
            //anchors.bottomMargin: UiConstants.DefaultMargin/2
            echoMode: TextInput.Password
            placeholderText: "Password"
            inputMethodHints: Qt.ImhNoAutoUppercase|Qt.ImhNoPredictiveText
            onTextChanged: {
                errorHighlight=false;
            }

            platformSipAttributes: SipAttributes {
                actionKeyLabel: qsTr("__launch__")
                //actionKeyHighlighted: true
            }

            Keys.onReturnPressed: {
                fnCheckPassword();
            }
        }
    }

    Image {
        id: idSeparator
        anchors.top: idPassInputItem.bottom
        height: 2
        width: parent.width
        visible: false
        source: "image://theme/meegotouch-groupheader-background"
    }

    Flickable {
        id: idLabelTopItem
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: idPassInputItem.bottom
        anchors.margins: UiConstants.DefaultMargin
        flickableDirection: Flickable.VerticalFlick
        contentHeight: idLabelTop.height
        clip: true

        Label {
            id: idLabelTop
            width: parent.width

            Component.onCompleted: {
                text=fnGetMessage();
            }
        }
    }

    ScrollDecorator {
        flickableItem: idLabelTopItem
    }

    tools: ToolBarLayout {
        visible: true

        ToolButton {
            text: qsTr("__launch__")
            width: children.width
            anchors.left: parent.left
            anchors.leftMargin: UiConstants.DefaultMargin
            onClicked: {
                fnCheckPassword();
            }
        }

        ToolIcon {
            iconSource: "image://theme/icon-s-description"
            anchors.right: parent.right
            onClicked: idAboutDialog.open()
        }
    }
}
