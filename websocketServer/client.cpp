#include "client.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

Client::Client(QObject *parent)
    : QObject{parent}
{}

void Client::invoke(const QString &methodName, const QVector<Parameter> &allParams)
{
    if (!webSocket || webSocket->state() != QAbstractSocket::ConnectedState)
        return;

    QJsonObject obj;
    obj["event"] = methodName;

    QJsonArray arr;
    for (const Parameter &p : allParams) {
        arr.append(QJsonValue::fromVariant(p.value)); // فرض می‌کنم Parameter یک QVariant نگه می‌داره
    }
    obj["params"] = arr;

    QJsonDocument doc(obj);
    webSocket->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
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
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON from client:" << message;
        return;
    }

    QJsonObject obj = doc.object();
    QString event = obj.value("event").toString();
    QVariantList params = obj.value("params").toArray().toVariantList();

    qDebug() << "Client" << clientId << "sent event:" << event << "params:" << params;

    // اینجا می‌تونید event رو به Context یا Hub پاس بدید
    //emit eventReceived(clientId, event, params);
}
