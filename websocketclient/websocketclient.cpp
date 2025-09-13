#include "websocketclient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QThread>

WebSocketClient::WebSocketClient(const QUrl &url, QObject *parent):QObject(parent),url(url)
{
    connect(&websocketClient, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&websocketClient, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &WebSocketClient::onSslErrors);
    connect(&websocketClient,&QWebSocket::stateChanged, this, &WebSocketClient::onStateChanged);
    connect(&websocketClient,&QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(&websocketClient, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&websocketClient, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, [&](QAbstractSocket::SocketError) {
        qWarning() << "WebSocket error:" << websocketClient.errorString();
    });
}

bool WebSocketClient::Connect()
{
    bool success = false;
    QEventLoop connectingloop;
    websocketClient.open(QUrl(url));

    // Connect signals for connection success and failure
    connect(&websocketClient, &QWebSocket::connected, &connectingloop, [&]() {
        success = true;
        connectingloop.quit();
    });

    connect(&websocketClient, &QWebSocket::disconnected, &connectingloop, [&]() {
        success = false;
        connectingloop.quit();
    });

    connect(&websocketClient, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), &connectingloop, [&](QAbstractSocket::SocketError) {
        qFatal() << "WebSocket error:" << websocketClient.errorString();
        success = false;
        connectingloop.quit();
    });

    connectingloop.exec();
    return success;
}

bool WebSocketClient::Disconnect()
{
    try {
        websocketClient.close(QWebSocketProtocol::CloseCodeNormal);
        return true;
    } catch (const std::exception &e) {
        qFatal() << "WebSocket error:" << websocketClient.errorString();
        return false;
    }
}


QAbstractSocket::SocketState WebSocketClient::getState() const
{
    return state;
}

void WebSocketClient::Invoke(QString methodName, QVector<Parameter>  parameters,std::function<void(const QVariant &)> callback)
{
    QJsonDocument InvoceJson;
    QJsonObject InvoceJsonObject;
    QUuid functionId = QUuid::createUuid();
    InvoceJsonObject["ClientId"] = ClientId.toString();
    InvoceJsonObject["FunctionId"] = functionId.toString();
    InvoceJsonObject["MessageType"] = MessageType::CallFunction;
    InvoceJsonObject["MethodName"] = methodName;
    QJsonArray ParamsJson;
    foreach (Parameter param, parameters) {
        QJsonObject paramJsonObject;
        paramJsonObject["ParamName"] = param.paramName;
        paramJsonObject["Value"] = QJsonValue::fromVariant(param.value);
        ParamsJson.append(paramJsonObject);
    }

    InvoceJsonObject["Params"] = ParamsJson;
    InvoceJson.setObject(InvoceJsonObject);
    QString jsonStr = QJsonDocument(InvoceJsonObject).toJson(QJsonDocument::Indented);
    jsonStr = jsonStr.replace("\n","").replace(" ","");
    if (websocketClient.state()== QAbstractSocket::SocketState::ConnectedState)
    {
        //websocketClient.resume();
        websocketClient.sendTextMessage(jsonStr);
        QThread::create([=]{
            while (!resultFunctions.contains(functionId));
            auto result = resultFunctions[functionId];
            resultFunctions.remove(functionId);
            callback(result);
        })->start();
    }
}

void WebSocketClient::On(const QString &methodName, std::function<void (QVariantList)> callback)
{
    // Wrap the callback to handle QVariant conversion
    auto wrapper = [callback](const QVariantList &params) {
        // Convert the QVariant to the expected type and invoke the callback
        callback(params);
    };

    // Store the callback in the map
    m_callbacks.insert(methodName, wrapper);
}

void WebSocketClient::callMethod(const QString &methodName, const QVariantList &params)
{
    if (m_callbacks.contains(methodName)) {
        m_callbacks[methodName](params);
    } else {
        qWarning() << "No callback registered for method:" << methodName;
    }
}

void WebSocketClient::onConnected()
{
    this->state = QAbstractSocket::SocketState::ConnectedState;
}

void WebSocketClient::onTextMessageReceived(QString message)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());

    // Check for parsing errors
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qWarning() << "Invalid JSON format.";
        return ;
    }

    // Extract the root JSON object
    QJsonObject jsonObj = jsonDoc.object();

    // Access individual fields
    MessageType messageType = (MessageType)jsonObj.value("MessageType").toInt();
    switch (messageType) {
        case Identity:{
            ClientId = QUuid(jsonObj.value("Id").toString());
            break;
        }
        case CallFunction:{
            QVariantList parameters;
            QJsonArray jsonArray = jsonObj.value("Params").toArray();
            for (const QJsonValue value : jsonArray) {
                QJsonObject  param = value.toObject();
                Parameter parameter;
                parameter.paramName = param.value("ParamName").toString();
                parameter.value = param.value("Value").toVariant();
                parameters.push_back(QVariant::fromValue(parameter));
            }
            callMethod(jsonObj.value("MethodName").toString(),parameters);
            break;
        }
        case Result:{
            auto functionIdStr = jsonObj.value("FunctionId").toString();
            resultFunctions[QUuid::fromString(functionIdStr)] = jsonObj.value("Value").toVariant();
            break;
        }
    }
}

void WebSocketClient::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors)

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    websocketClient.ignoreSslErrors();
}

void WebSocketClient::onStateChanged(QAbstractSocket::SocketState state)
{
    this->state = state;
}

void WebSocketClient::onDisconnected()
{
    this->state = QAbstractSocket::SocketState::UnconnectedState;
}
