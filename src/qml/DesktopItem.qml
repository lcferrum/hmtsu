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

Item {
    signal clicked
    signal pressAndHold

    property string title: ""
    property string image: ""
    property bool selected: false

    height: UiConstants.ListItemHeightDefault
    width: parent.width

    BorderImage {
        id: idBackgroundImage
        anchors.fill: parent
        visible: false
    }

    Row {
        anchors.fill: parent
        anchors.leftMargin: UiConstants.DefaultMargin
        anchors.rightMargin: UiConstants.DefaultMargin
        spacing: UiConstants.DefaultMargin

        Image {
            id: idAppIcon
            width: 64
            height: 64
            fillMode: Image.PreserveAspectFit
            smooth: true
            sourceSize.width: 64
            sourceSize.height: 64
            anchors.verticalCenter: parent.verticalCenter
            source: image

            onStatusChanged: {
                if (status===Image.Error)
                    source="image://theme/icon-l-default-application";
            }
        }

        Label {
            id: idNameLabel
            font: UiConstants.TitleFont
            color: "#282828"
            width: parent.width-idAppIcon.width-parent.spacing
            anchors.verticalCenter: parent.verticalCenter
            elide: Text.ElideRight
            text: title
        }
    }

    MouseArea {
        id: idClickArea
        anchors.fill: parent

        onClicked: parent.clicked()

        onPressAndHold: parent.pressAndHold()
    }

    states: [
        State {
            name: "highlighted"
            when: idClickArea.pressed
            PropertyChanges { target: idBackgroundImage; visible: true; source: "image://theme/meegotouch-panel-background-pressed" }
            PropertyChanges { target: idNameLabel; color: "#797979" }
        },
        State {
            name: "selected"
            when: selected&&!idClickArea.pressed
            PropertyChanges { target: idBackgroundImage; visible: true; source: "image://theme/color"+theme.colorScheme+"-meegotouch-panel-background-selected" }
            PropertyChanges { target: idNameLabel; color: "#FFFFFF" }
        }
    ]

    transitions: Transition {   //This transition acts as delay for state change between "default" and "highlighted"
        from: ""                //It ensures that there is no unnecessary "highlighted" state right before "selected"
        to: "highlighted"
        SequentialAnimation {
            PauseAnimation { duration: 70 }
            PropertyAction { targets: [idBackgroundImage, idNameLabel]; properties: "visible, source, color" }
        }
    }
}
