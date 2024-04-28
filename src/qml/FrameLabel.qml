import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Column {
    id: root

    default property alias content: innerPane.data
    property alias text: innerLabel.text

    spacing: 5
    padding: 5

    Label {
        id: innerLabel

        visible: text.length > 0
    }

    Frame {
        id: innerFrame

        anchors.horizontalCenter: parent.horizontalCenter
        width: root.width - (root.topPadding + root.bottomPadding)
        height: {
            if (innerLabel.visible) {
                return root.height - root.spacing - (root.leftPadding + root.rightPadding) - innerLabel.height
            }

            return root.height - (root.leftPadding + root.rightPadding)
        }

        Pane {
            id: innerPane

            anchors.fill: parent
        }
    }
}
