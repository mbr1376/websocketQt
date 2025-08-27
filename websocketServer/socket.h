#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QWebSocketServer>
class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);


signals:


private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onTextMessageReceived(const QString& message);


private:
    QWebSocketServer *m_server;
    QMap<QUuid,QWebSocket*> m_clients;
};

#endif // SOCKET_H
