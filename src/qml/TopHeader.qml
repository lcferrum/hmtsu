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

Item {
    property string text: ""
    property variant color: undefined
    property bool landscape: false

    height: landscape?UiConstants.HeaderDefaultHeightLandscape:UiConstants.HeaderDefaultHeightPortrait

    Loader {
        anchors.fill: parent
        sourceComponent: parent.color===undefined?idDefaultBg:idCustomBg
    }

    Component {
        id: idCustomBg

        Rectangle {
            anchors.fill: parent
            color: parent.parent.color
            Rectangle {
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 1
                opacity: 0.13
                color: "#000000"
            }
            Rectangle {
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
                color: "#FFFFFF"
            }
        }
    }

    Component {
        id: idDefaultBg

        Image {
            anchors.fill: parent
            source: "image://theme/meegotouch-view-header-fixed"
        }
    }

    Item {
        anchors.fill: parent
        anchors.topMargin: UiConstants.HeaderDefaultTopSpacingPortrait
        anchors.bottomMargin: UiConstants.HeaderDefaultBottomSpacingPortrait
        anchors.rightMargin: UiConstants.DefaultMargin
        anchors.leftMargin: UiConstants.DefaultMargin

        Label {
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
            font: UiConstants.HeaderFont
            color: parent.parent.color===undefined?"black":"white"
            text: parent.parent.text
            elide: Text.ElideRight
        }
    }
}
