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
    property string subtitle: ""
    property bool tumbler: false

    height: UiConstants.ListItemHeightDefault
    width: parent.width
    opacity: enabled?1:0.52

    BorderImage {
        anchors.fill: parent
        anchors.leftMargin: -UiConstants.DefaultMargin
        anchors.rightMargin: -UiConstants.DefaultMargin
        visible: idClickArea.pressed
        source: "image://theme/meegotouch-panel-background-pressed"
    }

    Row {
        anchors.fill: parent
        spacing: UiConstants.DefaultMargin

        Column {
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width-idIndicator.width-parent.spacing

            Label {
                font: UiConstants.TitleFont
                color: idClickArea.pressed?"#797979":"#282828"
                text: title
                width: parent.width
                elide: Text.ElideRight
            }

            Label {
                font: UiConstants.SubtitleFont
                color: idClickArea.pressed?"#797979":"#505050"
                text: subtitle
                visible: text.length>0
                width: parent.width
                elide: Text.ElideRight
            }
        }

        Image {
            id: idIndicator
            opacity: idClickArea.pressed?0.39:1
            anchors.verticalCenter: parent.verticalCenter
            source: tumbler?"image://theme/meegotouch-combobox-indicator":"image://theme/icon-m-common-drilldown-arrow"
        }
    }

    MouseArea {
        id: idClickArea
        anchors.fill: parent
        enabled: parent.enabled //"QGraphicsItem::ungrabMouse: not a mouse grabber" on clicks when disabled

        onClicked: parent.clicked()

        onPressAndHold: parent.pressAndHold()
    }
}
