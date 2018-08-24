#ifndef INSTANCEMESSENGER_H
#define INSTANCEMESSENGER_H

// QT
#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QAbstractSocket>

// FORWARD DECLARATIONS
QT_FORWARD_DECLARE_CLASS(TCPClient)
QT_FORWARD_DECLARE_CLASS(ContactsModel)
QT_FORWARD_DECLARE_CLASS(DialogModel)

// ANOTHERS
#include "src/instancemessenger_global.h"
#include "nlohmann/json.hpp"

struct UserItem {
public:
    UserItem() {}
    UserItem(const QString &_phone, const int &_id) :
        m_phone(_phone),
        m_id(_id) {}

    bool operator==(const UserItem &_other) const {
        return this->m_id == _other.m_id && this->m_phone == _other.m_phone;
    }

public:
    QString m_phone{"89264916734"};
    int m_id;

    // Unused yet
    QString m_nikName;
    QString m_firstName;
    QString m_secondName;
    bool m_isOnline{false};
};

struct DialogItem {
public:
    DialogItem() {}
    DialogItem(const QString &_message, const int &_userId) :
        m_message(_message),
        m_user_id(_userId) {}

    bool operator==(const DialogItem &_other) const {
        return this->m_message == _other.m_message && this->m_user_id == _other.m_user_id;
    }

public:
    QString m_message;
    int m_user_id;

    // Unused yet
    QString m_dialogName;
    bool m_isReceived;
    QDateTime m_dateTime;
};

class InstanceMessenger : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool authStatus READ getAuthStatus WRITE setAuthStatus NOTIFY authStatusChanged)
    Q_PROPERTY(QString authStatus READ getSocketStatus NOTIFY socketStatusChanged)

public:
    explicit InstanceMessenger(TCPClient *_tcpClient, QObject *_parent = nullptr);
    ~InstanceMessenger();

signals:
    Q_INVOKABLE void authStatusChanged();
    Q_INVOKABLE void socketStatusChanged();

public:
    Q_INVOKABLE void reconnect();
    Q_INVOKABLE void disconnect();

public:
    Q_INVOKABLE void sendHello();
    Q_INVOKABLE void sendRequestCode();
    Q_INVOKABLE void sendRequestJWT();
    Q_INVOKABLE void sendRequestPresence();
    Q_INVOKABLE void sendRequestContacts();
    Q_INVOKABLE void sendRequestDialog(const int &_userId);
    Q_INVOKABLE void sendMessage(const QString &_message, const int &_userId);

public:
    Q_INVOKABLE QString getCode() const { return m_code; }
    Q_INVOKABLE QString getSocketStatus() const { return m_socketState; }
    Q_INVOKABLE QString getTempToken() const { return m_tempToken; }
    Q_INVOKABLE QString getJWT() const { return m_jwt; }
    Q_INVOKABLE bool getAuthStatus() const { return m_authStatus; }
    Q_INVOKABLE QString getPhone() const { return m_clientData.m_phone; }
    Q_INVOKABLE QMap<int, UserItem> getContacts() const { return m_contacts; }
    Q_INVOKABLE QMap<int, QList<DialogItem> > getDialogs() const { return m_dialogs; }

    Q_INVOKABLE ContactsModel* getContactsModel() { return m_pContactsModel; }
    Q_INVOKABLE DialogModel* getDialogModel() { return m_pDialogModel; }

    Q_INVOKABLE int getActiveDialog() const { return m_activeDialog; }

public:
    Q_INVOKABLE void setCode(const QString &_code) { m_code = _code; }
    Q_INVOKABLE void setTempToken(const QString &_tempToken) { m_tempToken = _tempToken; }
    Q_INVOKABLE void setJWT(const QString &_jwt) { m_jwt = _jwt; }
    Q_INVOKABLE void setAuthStatus(const bool &_status) { m_clientData.m_phone = _status; emit authStatusChanged(); }
    Q_INVOKABLE void setPhone(const QString &_phone) { m_clientData.m_phone = _phone; }
    Q_INVOKABLE void setActiveDialog(const int &_userId);

public slots:
    void onSocketStateChanged(QAbstractSocket::SocketState _socketState);
    void onReceived(nlohmann::json _receivedPackage);

    std::string getCommand(const nlohmann::json &_json) const;
    std::string getDescription(const nlohmann::json &_json) const;
    int getCode(const nlohmann::json &_json) const;

private:
    void processResponseToRequestHello(nlohmann::json _json);
    void processResponseToRequestCode(nlohmann::json &_json);
    void processResponseToRequestJWT(nlohmann::json &_json);
    void processResponseToPresence(nlohmann::json &_json);
    void processResponseToContacts(nlohmann::json &_json);
    void processResponseToDialog(nlohmann::json &_json);
    void processResponseToSendMessage(nlohmann::json &_json);

    void processReceivedMessage(nlohmann::json &_json);

public:
    void readJWTFromFile();
    void writeJWTToFile();

private:
    TCPClient *m_pTCPClient{nullptr};   // socket
    QString m_socketState;
    QString m_code;                     // sms code (should received from server with code request)
    QString m_tempToken;                // temp token (should received from server with code request)
    QString m_jwt;                      // jwt token (should received from server with jwt request)
    bool m_authStatus{false};            // authorization status (should received from server with presence request)

private:
    UserItem m_clientData;                       // personal data   - phone and id
    QMap<int, UserItem> m_contacts;          // map of contacts - key - user_id, value - userItem obj
    QMap<int, QList<DialogItem> > m_dialogs; // map of dialogs  - key - user_id, value - list of dialogItem obj

public:
    int m_activeDialog;

public:
    ContactsModel *m_pContactsModel;
    DialogModel *m_pDialogModel;

    std::string m_worldStdString{""};
    QString m_jwtPath{"token.jwt"};



public:
    static nlohmann::json getJsonFromString(const QString &_strName, const QString &_str);
    static nlohmann::json getJsonFromString(const std::string &_strName, const std::string &_str);

public:
    nlohmann::json getPhoneJson() const;
};

#endif // INSTANCEMESSENGER_H
