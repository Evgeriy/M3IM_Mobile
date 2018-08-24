import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    visible: true
    width: 480
    height: 600
    title: qsTr("Tabs")

    Rectangle {
//            border.color: "#1e90ff"
//            border.width: 1
        width: parent.width
        height: parent.height
        color: "#100000FF"
    }


    Connections {
        target: tabAuthorization
        onSigNextPage: {
            swipeView.setCurrentIndex(1);
        }
    }

    Connections {
        target: tabFriends
        onSigNextPage: {
            swipeView.setCurrentIndex(2);
            tabMessages.setHeader(tabFriends.getHeader());
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        AuthorizationPage {
            id: tabAuthorization
        }

        FriendsPage {
            id: tabFriends
        }

        MessagesPage {
            id: tabMessages
        }

        Settings {
            id: tabSettings
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        Rectangle {
            width: parent.width
            height: parent.height
            color: "#100000FF"
        }


        TabButton {
            text: qsTr("Authorization")
            Rectangle {
                border.color: "#1e90ff"
                border.width: 1
                width: parent.width
                height: parent.height
                color: "#100000FF"
            }
        }

        TabButton {
            text: qsTr("Friends")
            Rectangle {
                border.color: "#1e90ff"
                border.width: 1
                width: parent.width
                height: parent.height
                color: "#100000FF"
            }

            onClicked: {
                client.sendRequestContacts()
            }
        }

        TabButton {
            text: qsTr("Messages")
            Rectangle {
                border.color: "#1e90ff"
                border.width: 1
                width: parent.width
                height: parent.height
                color: "#100000FF"
            }
        }

        TabButton {
            text: qsTr("Settings")
            Rectangle {
                border.color: "#1e90ff"
                border.width: 1
                width: parent.width
                height: parent.height
                color: "#100000FF"
            }
        }
    }
}
