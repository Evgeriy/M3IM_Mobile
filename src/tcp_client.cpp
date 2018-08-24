#include "tcp_client.h"

// QT
#include <QTime>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

// CPP/STL
#include <cstdio>
#include <iostream>

// ANOTHERS
//#include "../tests/mock_tcp_server.hpp"

void TCPClient::setMockTCPServer(AbstractTCPServer *_server) {
    qRegisterMetaType<nlohmann::json>("json");
//    m_pMockTCPServer = _server;
}

TCPClient::TCPClient(QObject *_parent) :
    QObject(_parent),
    m_pSocket(nullptr) {

    m_pSocket = new QTcpSocket();

    // connect signals and slots
    connect(m_pSocket, &QTcpSocket::connected, this, &TCPClient::onConnected);
    connect(m_pSocket, &QTcpSocket::readyRead, this, &TCPClient::onReceived);
    connect(m_pSocket, &QTcpSocket::stateChanged, this, &TCPClient::statusChanged);
}

TCPClient::~TCPClient() {
    delete m_pSocket;
//    delete m_pMockTCPServer;
}

void TCPClient::reconnect() {
    if (m_pSocket != nullptr) {
        delete m_pSocket;
    }

    m_pSocket = new QTcpSocket();
    connect(m_pSocket, &QTcpSocket::connected, this, &TCPClient::onConnected);
    connect(m_pSocket, &QTcpSocket::readyRead, this, &TCPClient::onReceived);
    connect(m_pSocket, &QTcpSocket::stateChanged, this, &TCPClient::statusChanged);

    m_pSocket->connectToHost(m_hostAddress, m_port);
}

void TCPClient::disconnect() {
    m_pSocket->disconnect();
    m_pSocket->close();

    emit statusChanged(m_pSocket->state());
}

void TCPClient::connectToHost(const QString &_hostAddressString, const quint16 &_port) {
    m_hostAddress = _hostAddressString;
    m_port = _port;

    // make connection
    m_pSocket->connectToHost(m_hostAddress, m_port);
}

void TCPClient::send(const nlohmann::json &_sendedPackage) {
    // pack to binary
    std::string jsonMsgpack = jsonToMsgpack(_sendedPackage);

    int length = (int)jsonMsgpack.size();
    QString lengthStr = QString::number(length).rightJustified(4, '0');

    jsonMsgpack = lengthStr.toStdString() + jsonMsgpack;

//    if (m_pMockTCPServer != nullptr) {
        // send to mock server and receive
//        m_pMockTCPServer->onReceived(jsonMsgpack);
//        onReceived();
//    } else {
        // send to server
        // print debug
        std::cout << "[Send] Json package: " << _sendedPackage << std::endl;
        std::cout << "[Send] Msgpack package: " << jsonMsgpack << std::endl;
        m_pSocket->write(jsonMsgpack.c_str(), jsonMsgpack.size());
//    }
}

void TCPClient::onConnected() {
    // print debug
    std::cout << "Client IP: " << m_pSocket->localAddress().toString().toStdString() << std::endl;
    std::cout << "Server IP: " << m_pSocket->peerAddress().toString().toStdString() << std::endl;
}

void TCPClient::onReceived() {
    std::string receivedPackageStdString = "";
//    if (m_pMockTCPServer != nullptr) {
//         use send() of mock server
//        receivedPackageStdString = m_pMockTCPServer->send();
//        std::cout << "[Receive] Package: " << receivedPackageStdString << std::endl;

//        m_nextBlockSize = static_cast<quint16>(std::atoi(receivedPackageStdString.substr(0, 4).c_str()));
//        // print debug
//        std::cout << "[Receive] Package size: " << m_nextBlockSize << std::endl;

//        receivedPackageStdString = receivedPackageStdString.substr(4, receivedPackageStdString.length() - 4);

//        if (m_nextBlockSize == receivedPackageStdString.length()) {
//            // print debug
//            std::cout << "[Receive] Good package!" << std::endl;
//        }
//    } else {
        // read from socket
        while (true) {
            if (!m_nextBlockSize) {
                m_nextBlockSize = std::atoi(m_pSocket->read(4).toStdString().c_str());
                std::cout << "[Receive] Package size: " << m_nextBlockSize << std::endl;
            }

            if (m_pSocket->size() < m_nextBlockSize) {
                break;
            }

            receivedPackageStdString = m_pSocket->read(m_nextBlockSize).toStdString();
            std::cout << "[Receive] Package: " << receivedPackageStdString << std::endl;

            m_nextBlockSize = 0;
            break;
        }
//    }

    // print debug
    std::cout << "[Receive] Msgpack package: " << receivedPackageStdString << std::endl;

    // unpack to json
    nlohmann::json unpacked = nlohmann::json::from_msgpack(receivedPackageStdString);

    // print debug
    std::cout << "[Receive] Json package: " << unpacked << std::endl;

    emit received(unpacked);
}

std::string TCPClient::jsonToMsgpack(const nlohmann::json &_json) {
    std::string retValue = "";
    nlohmann::json::to_msgpack(_json, retValue);
    return retValue;
}
