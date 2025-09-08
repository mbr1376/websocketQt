#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QUuid>
#include <QWebSocketServer>

#include "clienthub.h"
class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);

    ~Socket();

signals:


private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onTextMessageReceived(const QString& message);


private:
    QWebSocketServer *m_server;
    QMap<QUuid,QWebSocket*> m_clients;
    ClientHub m_clientHub;

};

#endif // SOCKET_H
