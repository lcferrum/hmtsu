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
                paddingRight: idBtnFileSelectIcon.width+15;
            }
            platformSipAttributes: SipAttributes {
                actionKeyLabel: qsTr("__done__")
                actionKeyHighlighted: true
            }

            Keys.onReturnPressed: {
                fnContinue();
            }

            Image {
                id: idBtnFileSelectIcon
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                source: "image://theme/icon-m-toolbar-directory"
                opacity: idBtnFileSelect.pressed?0.39:1
            }
        }

        MouseArea {
            id: idBtnFileSelect
            anchors.right: parent.right
            anchors.left: idCommandText.right
            anchors.leftMargin: -idCommandText.platformStyle.paddingRight+1
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onClicked: {
                if (idAppBrowserList.currentIndex!==-1)
                    idAppBrowserList.positionViewAtIndex(idAppBrowserList.currentIndex, ListView.Center);
                else
                    idAppBrowserList.positionViewAtBeginning();
                idCommandText.platformCloseSoftwareInputPanel();
                idAppBrowserSheet.open();
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
        width: parent.width
        flickableDirection: Flickable.VerticalFlick
        contentHeight: idRunOptions.height
        clip: true

        Column {
            id: idRunOptions
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: UiConstants.DefaultMargin
            anchors.rightMargin: UiConstants.DefaultMargin

            GroupHeader {
                text: qsTr("__common__")
            }

            OptionsItem {
                title: qsTr("__user__")
                subtitle: idDialogUser.model.get(idDialogUser.selectedIndex).name
                enabled: subtitle.length>0
                tumbler: true

                onClicked: idDialogUser.open()
            }

            OptionsItem {
                title: qsTr("__mode_select__")
                subtitle: idDialogMode.model.get(idDialogMode.selectedIndex).name
                enabled: subtitle.length>0
                tumbler: true

                onClicked: idDialogMode.open()
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

    Sheet {
        id: idAppBrowserSheet
        acceptButtonText: qsTr("__open_app__")
        rejectButtonText: qsTr("__cancel_app__")
        acceptButton.enabled: idAppBrowserList.currentIndex!==-1
        property int lastIdx: -1

        content: Item {
            anchors.fill: parent

            ListView {
                id: idAppBrowserList
                highlightFollowsCurrentItem: false
                anchors.fill: parent
                cacheBuffer: screen.displayWidth
                currentIndex: -1
                clip: true
                model: objAppList

                header: Label {
                    height: screen.currentOrientation===Screen.Landscape?53:58
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: UiConstants.DefaultMargin
                    anchors.rightMargin: UiConstants.DefaultMargin
                    verticalAlignment: Text.AlignVCenter
                    font: UiConstants.HeaderFont
                    color: "#282828"
                    text: qsTr("__select_app__")

                    BusyIndicator {
                        id: idSheetBusy
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false
                        running: visible

                        Connections {   //Should declare Connections here because idSheetBusy is not visible outside ListView's header
                            target: objAppList

                            onSignalModelBusy: idSheetBusy.visible=busy
                        }
                    }
                }

                delegate: DesktopItem {
                    title: name
                    image: icon
                    selected: ListView.isCurrentItem
                    property string file: path

                    onClicked: idAppBrowserList.currentIndex=index
                }
            }

            ScrollDecorator {
                flickableItem: idAppBrowserList
            }
        }

        onStatusChanged: {
            if (status===DialogStatus.Open)
                objAppList.PopulateList();
        }

        onAccepted: {
            lastIdx=idAppBrowserList.currentIndex;
            idCommandText.text=objContext.ForceDesktop(idAppBrowserList.currentItem.file);
            objIntercom.AddInfo(qsTr("__app_selected__"));
        }

        onRejected: idAppBrowserList.currentIndex=lastIdx
    }

    tools: ToolBarLayout {
        visible: true

        ToolIcon {
            iconId: "toolbar-done"

            onClicked: fnContinue()
        }

        ToolIcon {
            iconSource: "image://theme/icon-s-description"
            anchors.right: parent.right

            onClicked: idAboutDialog.open()
        }
    }
}
