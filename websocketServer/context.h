#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>
#include <QWebSocket>
#include "client.h"
class Context : public QObject
{
    Q_OBJECT
public:
    explicit Context(QObject *parent = nullptr);
    void addClient(const QUuid &clientId, QWebSocket* webSocket);
    void removeClient(const QUuid &clientId);
    QMap<QUuid, Client*> clients;

    void invokeAll(const QString& methodName, const QVector<Parameter>& allParams);
signals:
};

#endif // CONTEXT_H
