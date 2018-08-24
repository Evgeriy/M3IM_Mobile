import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: pageId
    width: 480
    height: 600
    signal sigNextPage()

    header: Label {
        id: heagerLabel
        height: 60

        RoundButton {
            background: Rectangle {
                color: "#100000FF"
                radius: 0
                border.width: 1
                border.color: "#1e90ff"
            }
            enabled: false
            width: parent.width
            height: parent.height
            focusPolicy: Qt.NoFocus
            anchors.horizontalCenter: parent.horizontalCenter
        }

        text: client.getAuthStatus() ? qsTr("Authorization complete") : qsTr("Registration")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        font.bold: true
        padding: 10
    }

    Connections {
        target: client
        onAuthStatusChanged: {
            heagerLabel.text = client.getAuthStatus() ? qsTr("Authorization complete") : qsTr("Registration")
            buttonRequestCode.visible = !buttonRequestCode.visible;
            console.log(buttonRequestCode.visible);
        }
    }

    GridLayout {
        id: gridLayoutAuthorizationPage

        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        rows: 4
        columns: 2
        columnSpacing: 30
        rowSpacing: 30

        Label {
            id: labelPhone

            text: qsTr("Phone:")
            font.pixelSize: Qt.application.font.pixelSize * 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 1
            Layout.column: 1
        }

        TextEdit {
            id: textEditPhone
            text: !client.getAuthStatus() ? "___" : client.getPhone()

            font.pixelSize: Qt.application.font.pixelSize * 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 1
            Layout.column: 2
        }

        RoundButton {
            background: Rectangle {
                color: "#100000FF"
                radius: 15
                border.width: 1
                border.color: "#1e90ff"
            }

            id: buttonRequestCode
            visible: !client.getAuthStatus()

            text: qsTr("Request Code")
            font.pixelSize: Qt.application.font.pixelSize * 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.row: 2
            Layout.column: 1

            onClicked: {
                client.setPhone(textEditPhone.text);
                client.sendRequestCode();
            }
        }

        Label {
            id: labelCode
            visible: !client.getAuthStatus()

            text: qsTr("Code:")
            font.pixelSize: Qt.application.font.pixelSize * 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 3
            Layout.column: 1
        }

        TextEdit {
            id: textEditCode
            text: "___"
            visible: !client.getAuthStatus()

            font.pixelSize: Qt.application.font.pixelSize * 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 3
            Layout.column: 2
        }

        RoundButton {
            background: Rectangle {
                color: "#100000FF"
                radius: 15
                border.width: 1
                border.color: "#1e90ff"
            }

            id: buttonRegister
            visible: !client.getAuthStatus()

            text: qsTr("Register")
            font.pixelSize: Qt.application.font.pixelSize * 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.row: 4
            Layout.column: 1

            onClicked: {
                client.setCode(textEditCode.text);
                client.sendRequestJWT();
            }
        }
    }




}
