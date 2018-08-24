#include "mock_tcp_server.hpp"

TEST(TCPClientTest, SendRequestContacts) {
    // create client, messenger, mock_server
    TCPClient *client = new TCPClient();
    InstanceMessenger *messenger = new InstanceMessenger(client);
    MockTCPServer *mock_server = new MockTCPServer();

    // set mock server to client
    client->setMockTCPServer(mock_server);

    nlohmann::json input = R"({
                           "command": "contacts"
                       })"_json;

    nlohmann::json output = R"({
                            "command": "contacts",
                            "code": 200,
                            "description": "ok",
                            "payload": {
                                "users": [
                                    {
                                        "phone": "79081234567",
                                        "user_id": "128268"
                                    },
                                    {
                                        "phone": "79261234567",
                                        "user_id": "128261"
                                    },
                                    {
                                        "phone": "79111234567",
                                        "user_id": "128262"
                                    }
                                ]
                            }
                        })"_json;

    // configure input contacts
    QMap<int, UserItem> inputContacts;
    inputContacts[128268] = UserItem("79081234567", 128268);
    inputContacts[128261] = UserItem("79261234567", 128261);
    inputContacts[128262] = UserItem("79111234567", 128262);

    // configure returned values
    std::string inputMsgpack   = TCPClient::jsonToMsgpack(input);
    std::string outputMsgpack  = TCPClient::jsonToMsgpack(output);

    int length = (int)inputMsgpack.size();
    QString lengthStr = QString::number(length).rightJustified(4, '0');

    inputMsgpack = lengthStr.toStdString() + inputMsgpack;

    length = (int)outputMsgpack.size();
    lengthStr = QString::number(length).rightJustified(4, '0');

    outputMsgpack = lengthStr.toStdString() + outputMsgpack;

    // mock behaviour settings
    EXPECT_CALL(*mock_server, onReceived(inputMsgpack));
    EXPECT_CALL(*mock_server, send()).WillRepeatedly(Return(outputMsgpack));

    // call function
    messenger->sendRequestContacts();

    // get output contacts
    QMap<int, UserItem> outputContacts = messenger->getContacts();
    QList<int> keys = outputContacts.keys();

    // check results
    for (int i = 0; i < keys.size(); ++i) {
        EXPECT_EQ(outputContacts[keys[i]].m_id, inputContacts[keys[i]].m_id);
        EXPECT_EQ(outputContacts[keys[i]].m_phone, inputContacts[keys[i]].m_phone);
    }

    delete messenger;
}
