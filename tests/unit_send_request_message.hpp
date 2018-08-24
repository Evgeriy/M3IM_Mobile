#ifndef UNIT_SEND_REQUEST_MESSAGE_H
#define UNIT_SEND_REQUEST_MESSAGE_H

#include "mock_tcp_server.hpp"

TEST(TCPClientTest, SendRequestMessage) {
    // create client, messenger, mock_server
    TCPClient *client = new TCPClient();
    InstanceMessenger *messenger = new InstanceMessenger(client);
    MockTCPServer *mock_server = new MockTCPServer();

    // set mock server to client
    client->setMockTCPServer(mock_server);

    QString phone  = "79081234567";
    QString result = "3872115D34DF2106135E1F9E4107CAB5C170E14BC56B9D4F941C031153B8017F";

    messenger->setPhone(phone);

    nlohmann::json input = R"({
                           "command": "send_message",
                               "payload": {
                                   "message": "test",
                                   "user_id": 128272
                               }
                       })"_json;

    nlohmann::json output = R"({"command": "send_message", "code": 200, "description": "ok"})"_json;

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
    messenger->sendMessage("test", 128272);

    delete messenger;
}


#endif // UNIT_SEND_REQUEST_MESSAGE_H
