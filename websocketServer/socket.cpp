#include "socket.h"

#include <QUuid>
#include <QWebSocket>

Socket::Socket(QObject *parent)
    : QObject{parent},
    m_server(new QWebSocketServer(QStringLiteral ("Server"),
                                    QWebSocketServer::NonSecureMode,this))
{
    if (m_server->listen(QHostAddress::Any,12345)){
        connect(m_server,&QWebSocketServer::newConnection,this,&Socket::onNewConnection);
    }

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

    // می‌توانید پیام خوش‌آمدگویی یا Echo بفرستید
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

}
