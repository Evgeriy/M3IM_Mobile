import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: pageId
    width: 480
    height: 600
    signal sigNextPage()

    header: Label {
        text: qsTr("Contacts")

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
    }

    function getHeader() {
        return String(client.getActiveDialog());
    }

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 4
        model: contacts

        topMargin: 10

        delegate: Item {
            x: 10
            width: parent.width
            height: 80

            Row {
                id: rowFriendItem
                width: parent.width
                height: parent.height
                spacing: 10

                Column {
                    id: columnAvatar
                    width: rowFriendItem.height / 1.2

                    RoundButton {
                        background: Rectangle {
                            color: model.online ? "#90ee90" : "#100000FF"
                            border.color: "#1e90ff"
                            border.width: 1
                            radius: 100
                        }
                        width: rowFriendItem.height / 1.2
                        height: rowFriendItem.height / 1.2
                    }
                }

                Column {
                    id: columnFriendInfo
                    width: rowFriendItem.width - (columnAvatar.width) - spacing * 3;

                    Text {
                        id: textContactId
                        text: model.userId
                        font.family: "Tahoma"
                        font.pixelSize: 18
                        font.bold: true
                    }

                    Text {
                        id: textContactPhone
                        text: model.phone
                        font.family: "Tahoma"
                        font.pixelSize: 15
                        font.bold: true
                    }

                    Text {
                        id: textLastMessage
                        text: "Last message"
                        font.family: "Tahoma"
                        font.pixelSize: 15
                    }

                    spacing: 5
                }
            }

            MouseArea {
                anchors.fill: rowFriendItem
                onClicked: {
                    client.setActiveDialog(model.userId);
                    console.log(client.getActiveDialog());
                    pageId.sigNextPage();
                }
            }
        }
    }
}
