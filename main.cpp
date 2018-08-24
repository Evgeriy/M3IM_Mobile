// QT
#include <QCoreApplication>
#include <QFile>
#include <QDebug>

// ANOTHERS
#include "tests/mock_tcp_server.hpp"

// TEST BUILD SETTINGS
#define TEST_MODE             0
#define TEST_REQUEST_CODE     1
#define TEST_REQUEST_JWT      1
#define TEST_REQUEST_PRESENCE 1
#define TEST_REQUEST_CONTACTS 1
#define TEST_REQUEST_DIALOG   0
#define TEST_REQUEST_MESSAGE  1

#if TEST_MODE
    #if TEST_REQUEST_CODE
        #include "unit_send_request_code.hpp"
    #endif
    #if TEST_REQUEST_JWT
        #include "unit_send_request_jwt.hpp"
    #endif
    #if TEST_REQUEST_PRESENCE
        #include "unit_send_request_presence.hpp"
    #endif
    #if TEST_REQUEST_CONTACTS
        #include "unit_send_request_contacts.hpp"
    #endif
    #if TEST_REQUEST_DIALOG
        #include "unit_send_request_dialog.hpp"
    #endif
    #if TEST_REQUEST_MESSAGE
        #include "unit_send_request_message.hpp"
    #endif
#endif

// CONSOLE MODE SETTINGS
#define CONSOLE_MODE 0

// QML MODE SETTINGS
#define QML_MODE 1

#if QML_MODE
    #include <QGuiApplication>
    #include <QQmlContext>
    #include <QQmlComponent>
    #include <QQmlApplicationEngine>

    #include "src/contacts_model.hpp"
    #include "src/dialogs_model.hpp"
//    #include "src/tcp_client.h"
//    #include "src/instancemessenger.h"
#endif

int main(int argc, char *argv[]) {

#if TEST_MODE
    QCoreApplication app(argc, argv);

    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();

#elif CONSOLE_MODE
    QCoreApplication app(argc, argv);

//    TCPClient *client = new TCPClient();
//    InstanceMessenger *messenger = new InstanceMessenger(client);
//    messenger->setJWT("eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9...");
//    messenger->writeJWTToFile();
//    messenger->sendRequestCode();

//    nlohmann::json tempJson = R"({"command" => "hello", "code" => 200, "description" => "ok", "payload" => {"message" => "world"}})"_json;

//    std::cout << tempJson["command"] << std::endl;

//    int t = 9;
//    char s[4];
//    sprintf(s, "%04d", t);


//    std::string ss(s);
//    int tt = std::atoi(ss.c_str());
//    std::cout << ss << " " << tt << std::endl;

#elif QML_MODE
    // create gui app
    QGuiApplication app(argc, argv);

    // create qml engine obj
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    // create tcp client and connection
    TCPClient *client = new TCPClient();
    client->connectToHost("192.168.8.107", 6000);

    // create messenger obj
    InstanceMessenger *messenger = new InstanceMessenger(client);

    // set messenger and models to qml context
    engine.rootContext()->setContextProperty("client", QVariant::fromValue(messenger));
    engine.rootContext()->setContextProperty("contacts", QVariant::fromValue(messenger->getContactsModel()));
    engine.rootContext()->setContextProperty("dialog", QVariant::fromValue(messenger->getDialogModel()));

#endif

    return app.exec();
}
