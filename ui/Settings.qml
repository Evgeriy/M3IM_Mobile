import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: pageSettings
    width: 480
    height: 600

    header: Label {
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

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        font.bold: true
        padding: 10
        text: "Settings"
    }

    Connections {
        target: client
        onSocketStatusChanged : {
            labelSocketStatus.text = String(client.getSocketStatus());
        }
    }

    GridLayout {
        id: gridLayoutSettingsPage
        width: 200

        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        rows: 3
        columns: 1
        columnSpacing: 0
        rowSpacing: 15

        Label {
            id: labelSocketStatus

            text: ""
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Qt.application.font.pixelSize * 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 1
            Layout.column: 1
        }

        RoundButton {
            id: buttonReconnect
            text: "Reconnect"
            background: Rectangle {
                color: "#100000FF"
                radius: 15
                border.width: 1
                border.color: "#1e90ff"
            }


            font.pixelSize: Qt.application.font.pixelSize * 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 2
            Layout.column: 1

            onClicked: {
                client.reconnect();
            }
        }

        RoundButton {
            id: buttonDisconnect
            text: "Disconnect"
            background: Rectangle {
                color: "#100000FF"
                radius: 15
                border.width: 1
                border.color: "#1e90ff"
            }

            font.pixelSize: Qt.application.font.pixelSize * 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.row: 3
            Layout.column: 1

            onClicked: {
                client.disconnect();
            }
        }
    }

}
