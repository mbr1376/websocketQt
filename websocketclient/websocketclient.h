#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QEventLoop>
#include <QUuid>
#include <QVector>
#include <QVariant>
/**
 * @brief messageType server 
 * 
 */
enum MessageType{Identity=0,Result=1,CallFunction=2};
/**
 * @brief struct parametr
 * get data as server paramName and value
 */
struct Parameter{
public:
    QString paramName;
    QVariant value;
};
Q_DECLARE_METATYPE(Parameter)

class WebSocketClient: public QObject
{
    Q_OBJECT
public:
    WebSocketClient(const QUrl &url, QObject *parent = nullptr);
    bool Connect();
    bool Disconnect();
    QAbstractSocket::SocketState getState() const;
    void On(const QString &methodName, std::function<void(QVariantList)> callback);
    void Invoke(QString methodName, QVector<Parameter> parameters, std::function<void (const QVariant &)> callback);
private:
    QUuid ClientId;


    QUrl url;
    QWebSocket websocketClient;
    QAbstractSocket::SocketState state;
    QHash<QUuid,QVariant> resultFunctions;
    QHash<QString, std::function<void(const QVariantList &)>> m_callbacks;
    void callMethod(const QString &methodName, const QVariantList &params);
private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);
    void onStateChanged(QAbstractSocket::SocketState state);
    void onDisconnected();
};

#endif // WEBSOCKETCLIENT_H
