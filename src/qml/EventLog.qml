import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ERDC

ColumnLayout {
    ListView {
        id: listView

        Layout.fillHeight: true
        Layout.fillWidth: true

        clip: true
        ScrollBar.vertical: ScrollBar {
            policy: listView.contentHeight > listView.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
        }
        boundsBehavior: Flickable.StopAtBounds
        model: EventLogAPI.eventsModel
        delegate: Item {
            required property date timestamp
            required property int severity
            required property string message

            width: ListView.view.width
            height: messageLabel.implicitHeight + 6

            MouseArea {
                anchors.fill: parent

                onDoubleClicked: {
                    ClipboardAPI.copyToSystemClipboard(message)
                }
            }

            RowLayout {
                anchors.fill: parent

                Label {
                    Layout.alignment: Qt.AlignLeft
                    Layout.minimumWidth: 80
                    Layout.maximumWidth: 80
                    Layout.fillHeight: true

                    text: {
                        switch (severity) {
                        case Enums.Info:
                            return "[ INFO ]"
                        case Enums.Warning:
                            return "[ WARNING ]"
                        case Enums.Error:
                            return "[ ERROR ]"
                        }

                        return "[ UNKNOWN ]"
                    }

                    color: {
                        switch (severity) {
                        case Enums.Info:
                            return "#7cb46b"
                        case Enums.Warning:
                            return "#ffffa4"
                        case Enums.Error:
                            return "#f4444e"
                        }

                        return "white"
                    }
                }

                Label {
                    Layout.alignment: Qt.AlignLeft
                    Layout.minimumWidth: 100
                    Layout.maximumWidth: 100
                    Layout.fillHeight: true

                    text: Qt.formatDateTime(timestamp, "hh:mm:ss zzz")
                }

                Label {
                    id: messageLabel

                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    wrapMode: Text.Wrap
                    text: message
                }
            }
        }

        onCountChanged: {
            currentIndex = count - 1
        }

        Label {
            anchors.centerIn: parent

            visible: listView.count === 0
            text: qsTr("Event log empty")
        }
    }

    RowLayout {
        spacing: 10

        Label {
            text: qsTr("Double click event to copy message to clipboard")
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            Layout.alignment: Qt.AlignRight

            text: qsTr("Clear events")

            onReleased: {
                EventLogAPI.clearEvents()
            }
        }
    }
}
