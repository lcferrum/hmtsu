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
    signal checkedChanged
    property alias checked: idSwitchArea.checked

    property string title: ""
    property string subtitle: ""

    height: UiConstants.ListItemHeightDefault
    width: parent.width

    Row {
        anchors.fill: parent
        spacing: UiConstants.DefaultMargin
        opacity: parent.enabled?1:0.52

        Column {
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width-idSwitchArea.width-UiConstants.DefaultMargin

            Label {
                font: UiConstants.TitleFont
                color: "#282828"
                text: title
                width: parent.width
                elide: Text.ElideRight
            }

            Label {
                font: UiConstants.SubtitleFont
                color: "#505050"
                text: subtitle
                visible: text.length>0
                width: parent.width
                elide: Text.ElideRight
            }
        }

        Switch {
            id: idSwitchArea
            anchors.verticalCenter: parent.verticalCenter
            enabled: parent.parent.enabled

            onCheckedChanged: parent.parent.checkedChanged()
        }
    }
}
