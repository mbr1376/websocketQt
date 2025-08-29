#include "client.h"

#include <QWebSocket>

Client::Client(QObject *parent)
    : QObject{parent}
{}

void Client::invoke(const QString &methodName, const QVector<Parameter> &allParams)
{

}

void Client::listen()
{
    connect(webSocket, &QWebSocket::textMessageReceived, this, &Client::handleTextMessage);
    connect(webSocket, &QWebSocket::disconnected, [this]() {
        emit onClose(clientId);
    });
}

void Client::handleTextMessage(const QString &message)
{

}
