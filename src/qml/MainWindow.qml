import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 1024
    height: 768
    minimumWidth: 640
    minimumHeight: 480

    visible: true
    title: qsTr("evemu-remote-device-control")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Edit")

            Action {
                text: qsTr("&Settings")

                onTriggered: {
                    rdcui.showSettingsDialog()
                }
            }
        }

        Menu {
            title: qsTr("&About")

            Action {
                text: qsTr("&Help")

                onTriggered: {
                    rdcui.showHelpDialog()
                }
            }
        }
    }

    RDCUI {
        id: rdcui

        anchors.fill: parent
    }
}
