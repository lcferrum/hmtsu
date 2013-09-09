import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    signal checkedChanged
    property alias checked: idSwitchArea.checked

    property string title: ""
    property string subtitle: ""

    height: UiConstants.ListItemHeightDefault
    width: parent.width

    Item {
        anchors.fill: parent
        opacity: parent.enabled?1:0.52

        Column {
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width-idSwitchArea.width-16

            Label {
                font: UiConstants.TitleFont
                color: "#282828"
                text: parent.parent.parent.title
                width: parent.width
                elide: Text.ElideRight
            }

            Label {
                font: UiConstants.SubtitleFont
                color: "#505050"
                text: parent.parent.parent.subtitle
                visible: text.length>0
                width: parent.width
                elide: Text.ElideRight
            }
        }
    }

    Item {
        anchors.right: parent.right
        height: parent.height

        Switch {
            id: idSwitchArea
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            enabled: parent.enabled
            onCheckedChanged: {
                parent.parent.checkedChanged();
            }
        }
    }

}
