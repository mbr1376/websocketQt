#include "socket.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>
#include <QWebSocket>
#include <QSslCertificate>
#include <QSslSocket>
#include <QFile>
#include <QSslKey>
#include <QDir>
#include "protocol.h"

Socket::Socket(QObject *parent)
    : QObject{parent},
#ifdef USE_WSS
    m_server(new QWebSocketServer(QStringLiteral ("Server"),
                                  QWebSocketServer::SecureMode, this))
#else
    m_server(new QWebSocketServer(QStringLiteral ("Server"),
                                  QWebSocketServer::NonSecureMode, this))
#endif
{
#ifdef USE_WSS
    QFile certFile(QDir::currentPath() + QStringLiteral("/wss/cert.pem"));
    QFile keyFile(QDir::currentPath() + QStringLiteral("/wss/key.pem"));
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::sslLibraryVersionString();
    qDebug() << "Supports SSL:" << QSslSocket::supportsSsl();
    QSslConfiguration sslConfig;
    if (certFile.open(QIODevice::ReadOnly) && keyFile.open(QIODevice::ReadOnly)) {
        QSslCertificate cert(&certFile, QSsl::Pem);
        QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem);
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        sslConfig.setLocalCertificate(cert);
        sslConfig.setPrivateKey(key);
        m_server->setSslConfiguration(sslConfig); // این خط را اضافه کنید
    }
    if (m_server->listen(QHostAddress::Any, 12345)) { // فقط دو آرگومان
        connect(m_server, &QWebSocketServer::newConnection, this, &Socket::onNewConnection);
    }
#else
    if (m_server->listen(QHostAddress::Any,12345)){
        connect(m_server,&QWebSocketServer::newConnection,this,&Socket::onNewConnection);
    }
#endif
    connect(&m_clientHub,&ClientHub::sendDataRealTime,[this](const QString &metodName,
            const QVector<Parameter> &params){
        if (m_clients.count() > 0){
            QMapIterator<QUuid,QWebSocket*> i(m_clients);
            while (i.hasNext()) {
                i.next();
                QString json = Protocol::getMethodInfoJson(metodName,params);
                if (i.value())
                    i.value()->sendTextMessage(json);
            }

        }
    });
    logger.setDateTimeFormat("yyyy/MM/dd hh:mm:ss");
    logger.setFormatTemplate("{datetime} [{level}] ({action}) -> {message}");
    m_clientHub.startRealTimeData();

}

Socket::~Socket()
{
    delete m_server;

    m_clients.clear();
}

void Socket::onNewConnection()
{
    QWebSocket* clientSocket = m_server->nextPendingConnection();
    QUuid clientId = QUuid::createUuid();
    m_clients[clientId] = clientSocket;
    logger.log(Logger::Action::Process,"new connection" + clientId.toString(),Logger::Level::Info);
    connect(clientSocket, &QWebSocket::disconnected,
            this, &Socket::onClientDisconnected);
    connect(clientSocket, &QWebSocket::textMessageReceived,
            this, &Socket::onTextMessageReceived);

    clientSocket->sendTextMessage(Protocol::getIdentityJson(clientId));

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
    auto metodInfo = Protocol::getMethodInfo(message);
    if (metodInfo.methodName == "getData"){
        QVariant result = m_clientHub.getValue(metodInfo.params[0].paramName, metodInfo.params[0].value.toInt());
        QString resultJson = Protocol::getResultJson(metodInfo.functionId,result);
        auto clientSocket = m_clients[metodInfo.clientId];
            if (clientSocket) {
            clientSocket->sendTextMessage(resultJson);
            logger.log(Logger::Action::Process,"get message: " + metodInfo.methodName + ":" +
                     metodInfo.params[0].paramName+"==>"+ metodInfo.params[0].value.toString() + metodInfo.clientId.toString() ,
                    Logger::Level::Info
                        );
        }
    }
}

