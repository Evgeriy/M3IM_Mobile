#include "mock_tcp_server.hpp"

TEST(TCPClientTest, SendRequestJWT) {
    // create client, messenger, mock_server
    TCPClient *client = new TCPClient();
    InstanceMessenger *messenger = new InstanceMessenger(client);
    MockTCPServer *mock_server = new MockTCPServer();

    // set mock server to client
    client->setMockTCPServer(mock_server);

    QString code  = "3802";
    QString tempToken = "3872115D34DF2106135E1F9E4107CAB5C170E14BC56B9D4F941C031153B8017F";
    QString result = "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9...";

    messenger->setCode(code);
    messenger->setTempToken(tempToken);

    nlohmann::json input = R"({
            "command": "request_jwt",
            "payload": {
                "type": "register",
                "code": "3802",
                "reset_password": false,
                "restore": false,
                "token": "3872115D34DF2106135E1F9E4107CAB5C170E14BC56B9D4F941C031153B8017F"
            }
        })"_json;

    nlohmann::json output = R"({
            "command": "request_jwt",
            "code": 200,
            "description": "ok",
            "payload": {
                "jwt": "eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9..."
            }
        })"_json;

//    // configure returned values
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
    messenger->sendRequestJWT();

    std::cout << std::endl << std::endl;
    std::cout << "[ACTUAL   TEMP TOKEN] " << messenger->getJWT().toStdString() << std::endl;
    std::cout << "[EXPECTED TEMP TOKEN] " << result.toStdString() << std::endl;

    // check results
    EXPECT_EQ(messenger->getJWT(), result);

    delete messenger;
}
