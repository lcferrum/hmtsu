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

Row {
    property string title: ""
    property string subtitle: ""
    property bool processing: false
    property bool landscape: false

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: UiConstants.DefaultMargin
    anchors.rightMargin: UiConstants.DefaultMargin
    height: idText.height+(landscape?11:16)
    spacing: UiConstants.DefaultMargin

    Column {
        id: idText
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width-idIndicator.width-parent.spacing

        Label {
            font: UiConstants.HeaderFont
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
        }
    }

    BusyIndicator {
        id: idIndicator
        anchors.verticalCenter: parent.verticalCenter
        opacity: running?1:0    //Using opacity here, instead of visibility, so that Row will be properly aligned
        running: processing
    }
}
