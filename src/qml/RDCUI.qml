import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ERDC

Pane {
    function showSettingsDialog() {
        settingsDialog.visible = true
    }

    function showHelpDialog() {
        helpDialog.visible = true
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 10

        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 200

            title: qsTr("Target device")

            Device {
                anchors.fill: parent
            }
        }

        GroupBox {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            Layout.alignment: Qt.AlignTop

            title: qsTr("Event log")

            EventLog {
                anchors.fill: parent
            }
        }
    }

    SettingsDialog {
        id: settingsDialog

        anchors.centerIn: parent
        width: Math.floor(ApplicationWindow.window.width * 0.8)
        height: Math.floor(ApplicationWindow.window.height * 0.8)
    }

    HelpDialog {
        id: helpDialog

        anchors.centerIn: parent
        width: Math.floor(ApplicationWindow.window.width * 0.8)
        height: Math.floor(ApplicationWindow.window.height * 0.8)
    }
}
