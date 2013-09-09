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
