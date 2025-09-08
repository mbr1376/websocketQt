#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QUuid>
#include <QVariant>
#include <QWebSocket>
#include "protocol.h"
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    QUuid clientId;
    QWebSocket* webSocket = nullptr;

    virtual void invoke(const QString& methodName, const QVector<Parameter>& allParams);
public slots:
     void listen();
signals:
      void onClose(const QUuid& clientId);


  private slots:
      void handleTextMessage(const QString& message);
};

#endif // CLIENT_H
