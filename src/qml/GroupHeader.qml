import QtQuick 1.1
import com.nokia.meego 1.0

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
