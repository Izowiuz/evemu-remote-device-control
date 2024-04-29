import QtQuick
import QtQuick.Controls

import ERDC

Item {
    id: root

    readonly property size mouseMarkerSize: Qt.size(30, 30)
    property QtObject currentMouseMarker
    property int pressedKeys: 0
    property bool keyboardInputEnabled: SettingsAPI.settings.keyboardDeviceName.length !== 0
                                        && root.activeFocus
                                        && !DeviceControlAPI.isProcessingEvent
    property bool mouseInputEnabled: SettingsAPI.settings.mouseDeviceName.length !== 0
                                     && !DeviceControlAPI.isProcessingEvent
    property bool blockUntillNoPressedKeys: false

    clip: true
    activeFocusOnTab: true
    focus: true

    function mouseEvent(mouseEvent) {
        if (!root.mouseInputEnabled) {
            return;
        }

        let xPos = mouseEvent.x - root.mouseMarkerSize.width / 2
        let yPos = mouseEvent.y - root.mouseMarkerSize.height / 2

        if (root.currentMouseMarker) {
            root.currentMouseMarker.destroy();
        }

        let obj = mouseMarker.createObject(targetRectangle, {
                                               x: xPos,
                                               y: yPos,
                                               ctrl: (mouseEvent.modifiers & Qt.ControlModifier) > 0,
                                               width: root.mouseMarkerSize.width,
                                               height: root.mouseMarkerSize.height
                                           })
        root.currentMouseMarker = obj

        DeviceControlAPI.mouseEvent(
            mouseEvent.buttons,
            mouseEvent.modifiers,
            Qt.point(mouseEvent.x, mouseEvent.y)
        )
    }

    function keyboardEvent(keyboardEvent) {
        if (!root.keyboardInputEnabled) {
            return;
        }

        DeviceControlAPI.keyboardEvent(
            keyboardEvent.key,
            keyboardEvent.modifiers
        )
    }


    Keys.onPressed: (event) => {
        if (event.isAutoRepeat) {
            return
        }

        root.pressedKeys++
        event.accepted = true;
    }

    Keys.onReleased: (event) => {
        if (event.isAutoRepeat) {
            return
        }

        root.pressedKeys--

        if (!root.blockUntillNoPressedKeys) {
            root.keyboardEvent(event);
            event.accepted = true;
        }

        if (root.pressedKeys === 0) {
            root.blockUntillNoPressedKeys = false
        }
    }

    Column {
        anchors {
            margins: 15
            top: parent.top
            left: parent.left
        }
        z: 100

        Label {
            font {
                bold: true
                pointSize: 13
            }
            text: root.mouseInputEnabled ?
                      qsTr("Mouse input enabled")
                    : qsTr("Mouse input disabled")
            color: root.mouseInputEnabled ?
                       "#7cb46b"
                     : "#f4444e"
            opacity: 0.74
        }

        Label {
            font {
                bold: true
                pointSize: 13
            }
            text: root.keyboardInputEnabled ?
                      qsTr("Keyboard input enabled")
                    : qsTr("Keyboard input disabled")
            color: root.keyboardInputEnabled ?
                       "#7cb46b"
                     : "#f4444e"
            opacity: 0.74
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        width: Math.floor(parent.height * 0.65)
        height: width
        running: DeviceControlAPI.isProcessingEvent
        enabled: !running
        visible: running
        z: 99
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true

        onReleased: {
            root.forceActiveFocus()
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border {
            width: 1
            color: "#7cb46b"
        }
        z: 98

        visible: root.activeFocus
    }

    Component {
        id: mouseMarker

        Rectangle {
            required property bool ctrl
            property color markerColor: ctrl ? "green" : "black"

            color: "transparent"
            border {
                color: markerColor
                width: 4
            }
            radius: width / 2
            opacity: 0.64
        }
    }

    Rectangle {
        id: targetRectangle

        anchors.centerIn: parent
        width: SettingsAPI.settings.deviceWidth
        height: SettingsAPI.settings.deviceHeight

        MouseArea {
            id: targetRectangleMOuseArea

            anchors.fill: parent
            enabled: SettingsAPI.settings.mouseDeviceName.length > 0
            hoverEnabled: true

            onReleased: (event) => {
                if (!root.activeFocus) {
                    root.forceActiveFocus()
                }

                if (event.x < 0 || event.y < 0 || event.x > targetRectangle.width || event.y > targetRectangle.height) {
                    return;
                }

                if (root.pressedKeys > 0) {
                    root.blockUntillNoPressedKeys = true
                }

                root.mouseEvent(event)
            }

            onPositionChanged: (mouse) => {
                positionMarker.x = mouse.x + 15
                positionMarker.y = mouse.y + 15
            }

             Label {
                 id: positionMarker

                 font {
                     bold: true
                     pointSize: 32
                 }

                 text: `[${parseInt(x - 15)}, ${parseInt(y - 15)}]`
                 visible: targetRectangleMOuseArea.containsMouse
                 color: "black"
                 opacity: 0.64
             }
        }

        Label {
            id: dimensions

            anchors.centerIn: parent

            font {
                bold: true
                pointSize: 32
            }
            text: `${parent.width}x${parent.height}`
            color: "black"
            opacity: 0.5
        }
    }
}
