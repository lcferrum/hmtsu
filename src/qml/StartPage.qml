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

    function fnContinue() {
        if (idCommandText.text.length<=0)
            objIntercom.AddWarning(qsTr("__command_text_empty__"));

        objContext.Mode=idDialogMode.model.get(idDialogMode.selectedIndex).mode;
        objContext.TargetUser=idDialogUser.model.get(idDialogUser.selectedIndex).name;
        objContext.SetCommand(idCommandText.text);
        pageStack.replace(idPassPage);
    }

    TopHeader {
        id: idBannerTop
        color: "#7DA4D3"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        landscape: screen.currentOrientation===Screen.Landscape
        text: qsTr("__start__")
    }

    Item {
        id: idCommandItem
        anchors.top: idBannerTop.bottom
        height: 80
        width: parent.width

        TextField {
            id: idCommandText
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: UiConstants.DefaultMargin
            anchors.rightMargin: UiConstants.DefaultMargin
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: qsTr("__run__")
            inputMethodHints: Qt.ImhNoAutoUppercase|Qt.ImhNoPredictiveText
            platformStyle: TextFieldStyle {
                //paddingRight: idBtnFileSelect.parent.width+15; //TODO: file select
            }
            Keys.onReturnPressed: {
                fnContinue();
            }

            platformSipAttributes: SipAttributes {
                //actionKeyLabel: qsTr("__launch__")
                //actionKeyHighlighted: true
                actionKeyIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-toolbar-done-white.png"
            }

            Image {
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                source: "image://theme/icon-m-toolbar-directory"
                opacity: idBtnFileSelect.pressed?0.39:1
                visible: false //TODO: file select
                MouseArea {
                    id: idBtnFileSelect
                    anchors.fill: parent
                    onClicked: {
                        //inputContext.reset(); //TODO: file select
                    }
                }
            }
        }
    }

    Image {
        id: idSeparator
        anchors.top: idCommandItem.bottom
        height: 2
        width: parent.width
        source: "image://theme/meegotouch-groupheader-background"
    }

    Flickable {
        anchors.top: idSeparator.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: UiConstants.DefaultMargin
        anchors.rightMargin: UiConstants.DefaultMargin
        flickableDirection: Flickable.VerticalFlick
        contentHeight: idRunOptions.height
        clip: true

        Column {
            id: idRunOptions
            width: parent.width

            GroupHeader {
                text: qsTr("__common__")
            }

            OptionsItem {
                title: qsTr("__user__")
                subtitle: idDialogUser.model.get(idDialogUser.selectedIndex).name
                enabled: subtitle.length>0
                tumbler: true
                onClicked: {
                    idDialogUser.open();
                }
            }

            OptionsItem {
                title: qsTr("__mode_select__")
                subtitle: idDialogMode.model.get(idDialogMode.selectedIndex).name
                enabled: subtitle.length>0
                tumbler: true
                onClicked: {
                    idDialogMode.open();
                }
            }

            GroupHeader {
                text: qsTr("__advanced__")
            }

            OptionsSwitch {
                title: qsTr("__login_switch__")
                checked: objContext.GetLogin()
                onCheckedChanged: objContext.SetLogin(checked)
            }

            OptionsSwitch {
                title: qsTr("__env_switch__")
                checked: objContext.GetPreserveEnv()
                onCheckedChanged: objContext.SetPreserveEnv(checked)
            }
        }
    }

    SelectionDialog {
        id: idDialogMode
        titleText: qsTr("__modes_list__")
        selectedIndex: model.GetInitialIndex()

        model: objModesList
    }

    SelectionDialog {
        id: idDialogUser
        titleText: qsTr("__users_list__")
        selectedIndex: model.GetInitialIndex()

        model: objUsersList
    }

    tools: ToolBarLayout {
        visible: true

        ToolIcon {
            iconId: "toolbar-done"
            onClicked: {
                fnContinue();
            }
        }

        ToolIcon {
            iconSource: "image://theme/icon-s-description"
            anchors.right: parent.right
            onClicked: idAboutDialog.open()
        }
    }
}
