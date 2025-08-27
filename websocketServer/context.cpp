#include "context.h"

Context::Context(QObject *parent)
    : QObject{parent}
{}

void Context::addClient(const QUuid &clientId, QWebSocket *QWebSocket)
{

}

void Context::removeClient(const QUuid &clientId)
{

}

void Context::invokeAll(const QString &methodName, const QVector<Parameter> &allParams)
{

}
