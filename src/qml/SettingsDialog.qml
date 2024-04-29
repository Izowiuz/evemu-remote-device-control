import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ERDC

Dialog {
    id: dialog

    title: qsTr("Settings")
    dim: true
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        SettingsAPI.settings.mouseDeviceName = mouseDeviceName.textField.text
        SettingsAPI.settings.keyboardDeviceName = keyboardDeviceName.textField.text
        SettingsAPI.settings.deviceWidth = deviceWidth.textField.text
        SettingsAPI.settings.deviceHeight = deviceHeight.textField.text
        SettingsAPI.settings.host = host.textField.text
        SettingsAPI.settings.user = user.textField.text
        SettingsAPI.settings.password = password.textField.text
        SettingsAPI.settings.identityFile = identityFile.textField.text
        SettingsAPI.settings.sshAskPass = sshAskPass.textField.text
        SettingsAPI.settings.xScaleFactor = xScaleFactor.textField.text
        SettingsAPI.settings.yScaleFactor = yScaleFactor.textField.text
        SettingsAPI.settings.customKeyboardMappings = customKeyboardMappings.text

        EventLogAPI.logEvent(Enums.Info, "Settings saved")
    }

    onRejected: {
        EventLogAPI.logEvent(Enums.Info, "Settings rejected")
    }

    onVisibleChanged: {
        if (visible) {
            mouseDeviceName.textField.text = SettingsAPI.settings.mouseDeviceName
            keyboardDeviceName.textField.text = SettingsAPI.settings.keyboardDeviceName
            deviceWidth.textField.text = SettingsAPI.settings.deviceWidth
            deviceHeight.textField.text = SettingsAPI.settings.deviceHeight
            host.textField.text = SettingsAPI.settings.host
            user.textField.text = SettingsAPI.settings.user
            identityFile.textField.text = SettingsAPI.settings.identityFile
            password.textField.text = SettingsAPI.settings.password
            sshAskPass.textField.text = SettingsAPI.settings.sshAskPass
            xScaleFactor.textField.text = SettingsAPI.settings.xScaleFactor
            yScaleFactor.textField.text = SettingsAPI.settings.yScaleFactor
            customKeyboardMappings.text = SettingsAPI.settings.customKeyboardMappings
        }
    }

    ColumnLayout {
        id: layout

        anchors.fill: parent
        spacing: 10

        GroupBox {
            Layout.fillWidth: true

            title: qsTr("Device")

            RowLayout {
                anchors.fill: parent

                spacing: 10

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    spacing: 10

                    TextFieldSetting {
                        id: mouseDeviceName
                        Layout.fillWidth: true
                        labelText: qsTr("Mouse device name")
                    }

                    TextFieldSetting {
                        id: deviceWidth
                        Layout.fillWidth: true
                        labelText: qsTr("Device screen width")
                        textField.validator: IntValidator {
                            bottom: 0
                            top: 10000
                        }
                    }

                    TextFieldSetting {
                        id: yScaleFactor
                        Layout.fillWidth: true
                        labelText: qsTr("Y scale factor")
                        textField.validator: DoubleValidator {
                            bottom: 0.0001
                            top: 10000
                            locale: "C"
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    spacing: 10

                    TextFieldSetting {
                        id: keyboardDeviceName
                        Layout.fillWidth: true
                        labelText: qsTr("Keyboard device name")
                    }

                    TextFieldSetting {
                        id: deviceHeight
                        Layout.fillWidth: true
                        labelText: qsTr("Device screen height")
                        IntValidator {
                            bottom: 0
                            top: 10000
                        }
                    }

                    TextFieldSetting {
                        id: xScaleFactor
                        Layout.fillWidth: true
                        labelText: qsTr("X scale factor")
                        textField.validator: DoubleValidator {
                            bottom: 0.0001
                            top: 10000
                            locale: "C"
                        }
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true

            title: qsTr("SSH")

            RowLayout {
                anchors.fill: parent
                spacing: 10

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    spacing: 10

                    TextFieldSetting {
                        id: host
                        Layout.fillWidth: true
                        labelText: qsTr("Host")
                    }

                    TextFieldSetting {
                        id: identityFile
                        Layout.fillWidth: true
                        labelText: qsTr("Identity file (<b>preferred way<b>)")
                    }

                    TextFieldSetting {
                        id: password
                        Layout.fillWidth: true
                        labelText: qsTr("Password (not saved in settings)")
                        textField.echoMode: TextInput.Password
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    spacing: 10

                    TextFieldSetting {
                        id: user
                        Layout.fillWidth: true
                        labelText: qsTr("User")
                    }

                    TextFieldSetting {
                        id: sshAskPass
                        Layout.fillWidth: true
                        labelText: qsTr("SSH_ASKPASS (<b>obligatory when using identity file<b>)")
                    }

                    Item {
                        Layout.fillHeight: true
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true

            title: qsTr("Custom keyboard mappings - JSON array of objects, example - [ {\"qt\": \"Key_Control\", \"ev\": \"KEY_CTRL\"} ]")

            Flickable {
                id: flickable

                anchors.fill: parent
                contentWidth: customKeyboardMappings.contentWidth
                contentHeight: customKeyboardMappings.implicitHeight
                ScrollBar.vertical: ScrollBar {
                    policy: flickable.contentHeight > flickable.height ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
                }
                boundsBehavior: Flickable.StopAtBounds

                clip: true

                function ensureVisible(r)
                {
                    if (contentX >= r.x) {
                        contentX = r.x;
                    } else if (contentX + width <= r.x + r.width) {
                        contentX = r.x + r.width - width;
                    }

                    if (contentY >= r.y) {
                        contentY = r.y;
                    } else if (contentY+height <= r.y + r.height) {
                        contentY = r.y + r.height - height;
                    }
                }

                TextField {
                    id: customKeyboardMappings

                    width: flickable.width
                    focus: true
                    wrapMode: TextEdit.Wrap
                    onCursorRectangleChanged: flickable.ensureVisible(cursorRectangle)
                }
            }
        }
    }
}
