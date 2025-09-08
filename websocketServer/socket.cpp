#include "socket.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>
#include <QWebSocket>

#include "protocol.h"

Socket::Socket(QObject *parent)
    : QObject{parent},
    m_server(new QWebSocketServer(QStringLiteral ("Server"),
                                    QWebSocketServer::NonSecureMode,this))
{
    if (m_server->listen(QHostAddress::Any,12345)){
        connect(m_server,&QWebSocketServer::newConnection,this,&Socket::onNewConnection);
    }

}

Socket::~Socket()
{
    delete m_server;

    m_clients.clear();
}

void Socket::onNewConnection()
{
    QWebSocket* clientSocket = m_server->nextPendingConnection();
    QUuid clientId = QUuid::createUuid();
    m_clients[clientId] = clientSocket;

    connect(clientSocket, &QWebSocket::disconnected,
            this, &Socket::onClientDisconnected);
    connect(clientSocket, &QWebSocket::textMessageReceived,
            this, &Socket::onTextMessageReceived);

    clientSocket->sendTextMessage(Protocol::getIdentityJson(clientId));

}

void Socket::onClientDisconnected()
{
    QWebSocket* clientSocket = qobject_cast<QWebSocket*>(sender());
    auto it = std::find_if(m_clients.begin(), m_clients.end(),
                           [clientSocket](const QWebSocket* ws) { return ws == clientSocket; });
    if (it != m_clients.end()) {
        m_clients.erase(it);
    }
    clientSocket->deleteLater();
}

void Socket::onTextMessageReceived(const QString &message)
{
    auto metodInfo = Protocol::getMethodInfo(message);
    if (metodInfo.methodName == "getData"){
        QVariant result = m_clientHub.getValue(metodInfo.params[0].paramName, metodInfo.params[0].value.toInt());
        QString resultJson = Protocol::getResultJson(metodInfo.functionId,result);
        auto clientSocket = m_clients[metodInfo.clientId];
            if (clientSocket) {
            clientSocket->sendTextMessage(resultJson);
        }
    }
}

