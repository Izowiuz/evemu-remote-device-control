import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    property alias labelText: label.text
    property alias textField: textField

    Label {
        id: label
        text: qsTr("Mouse device name")
    }

    TextField {
        id: textField
        Layout.fillWidth: true
        placeholderText: qsTr("...")
    }
}
