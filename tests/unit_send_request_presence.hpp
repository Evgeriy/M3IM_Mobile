#include "mock_tcp_server.hpp"

TEST(TCPClientTest, SendPresence) {
    // create client, messenger, mock_server
    TCPClient *client = new TCPClient();
    InstanceMessenger *messenger = new InstanceMessenger(client);
    MockTCPServer *mock_server = new MockTCPServer();

    // set mock server to client
    client->setMockTCPServer(mock_server);

    QString jwt = "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9...";
    bool auth_status = true;

    messenger->setJWT(jwt);

    nlohmann::json input = R"({
                           "command": "presence",
                           "payload": {
                               "jwt": "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9..."
                           }
                       })"_json;

    nlohmann::json output = R"({"command": "presence", "code": 200, "description": "ok"})"_json;

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
    messenger->sendRequestPresence();

    std::cout << std::endl << std::endl;
    std::cout << "[ACTUAL   TEMP TOKEN] " << messenger->getAuthStatus() << std::endl;
    std::cout << "[EXPECTED TEMP TOKEN] " << auth_status << std::endl;

    // check results
    EXPECT_EQ(messenger->getAuthStatus(), auth_status);

    delete messenger;
}
