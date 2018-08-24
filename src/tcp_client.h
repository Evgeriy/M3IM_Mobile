#ifndef AUTHORIZATIONDATA_H
#define AUTHORIZATIONDATA_H

// QT
#include <QObject>
#include <QAbstractSocket>

// FORWARD DECLARATIONS
QT_FORWARD_DECLARE_CLASS(QTcpSocket)
//QT_FORWARD_DECLARE_CLASS(MockTCPServer)

// ANOTHERS
#include "nlohmann/json.hpp"

class AbstractTCPServer {
public:
    virtual ~AbstractTCPServer() {}
    virtual std::string send() = 0;
    virtual std::string onReceived(std::string msg) = 0;
};

class TCPClient : public QObject {
    Q_OBJECT

public:
    void setMockTCPServer(AbstractTCPServer *_server);

public:
    explicit TCPClient(QObject *_parent = nullptr);
    virtual ~TCPClient();

public:
    void reconnect();
    void disconnect();

signals:
    void closed();
    void received(nlohmann::json);
    void statusChanged(QAbstractSocket::SocketState);

public:
    void connectToHost(const QString &_hostAddressString, const quint16 &_port);
    virtual void send(const nlohmann::json &_sendedPackage);

private slots:
    void onConnected();
    virtual void onReceived();

public:
    static std::string jsonToMsgpack(const nlohmann::json &_json);

private:
    QString m_hostAddress{""};
    quint16 m_port{0};

    QTcpSocket  *m_pSocket{nullptr};
//    AbstractTCPServer *m_pMockTCPServer{nullptr};
    quint16 m_nextBlockSize{0};
};


#endif
