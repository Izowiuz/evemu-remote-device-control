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

    Label {
        anchors.fill: parent

        font {
            pixelSize: 15
        }

        textFormat: Text.MarkdownText
        text: FileAPI.readFile(":/src/Help.md")
    }
}
