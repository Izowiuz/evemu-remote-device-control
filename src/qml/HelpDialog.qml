import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ERDC

Dialog {
    id: dialog

    title: qsTr("Help")
    dim: true
    modal: true
    standardButtons: Dialog.Ok

    Flickable {
        id: flickable

        anchors.fill: parent

        clip: true
        boundsBehavior: Flickable.StopAtBounds
        contentHeight: helpText.implicitHeight
        ScrollBar.vertical: ScrollBar {
            policy: flickable.contentHeight > flickable.height ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
        }

        Label {
            id: helpText

            width: parent.width
            font {
                pixelSize: 15
            }

            textFormat: Text.MarkdownText
            text: FileAPI.readFile(":/src/Help.md")
            wrapMode: Text.Wrap
        }
    }
}
