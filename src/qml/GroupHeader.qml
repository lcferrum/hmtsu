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
    property string text: ""
    property bool textOnly: false

    width: parent.width
    height: 40

    Image {
        anchors.right: text.length>0?idGroupName.left:parent.right
        anchors.left: parent.left
        anchors.rightMargin: text.length>0?24:0
        anchors.top: parent.top
        anchors.topMargin: 25
        visible: !parent.textOnly
        source: "image://theme/meegotouch-groupheader-background"
    }

    Label {
        id: idGroupName
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3
        anchors.right: parent.right
        font: UiConstants.GroupHeaderFont
        text: parent.text
        color: "#8c8c8c"
    }
}
