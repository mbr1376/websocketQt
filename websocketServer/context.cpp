#include "context.h"

Context::Context(QObject *parent)
    : QObject{parent}
{}

void Context::addClient(const QUuid &clientId, QWebSocket *webSocket)
{
    auto* client = new Client(this);
    client->clientId = clientId;
    client->webSocket = webSocket;
    clients[clientId] = client;
    connect(client, &Client::onClose, this, &Context::removeClient);
    client->listen();
}

void Context::removeClient(const QUuid &clientId)
{
    if (clients.contains(clientId)) {
        clients[clientId]->deleteLater();
        clients.remove(clientId);
    }
}

void Context::invokeAll(const QString &methodName, const QVector<Parameter> &allParams)
{
    for (auto* client : clients) {
        if (client->webSocket && client->webSocket->state() == QAbstractSocket::ConnectedState)
            client->invoke(methodName, allParams);
    }
}
