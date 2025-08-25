#ifndef PROTOCOL_H
#define PROTOCOL_H
#pragma once

#include <QJsonObject>
#include <QString>



enum class MessageType{
    echo =-1,
    Identity=0,
    Result =1,
    CallFunction =2

};

struct Echo
{
    MessageType messageType = MessageType::echo;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["MessageType"] = static_cast<int>(messageType);
        return obj;
    }
    static Echo fromJson(const QJsonObject &obj){
        return Echo{};
    }
};
struct Result
{
    QVariant value;
    QUuid functionId;
    MessageType messageType = MessageType::Result;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["Value"] = QJsonValue::fromVariant(value);
        obj["FunctionId"] = functionId.toString();
        obj["MessageType"] = static_cast<int>(messageType);
        return obj;
    }
    static Result fromJson(const QJsonObject& obj) {
        Result r;
        r.value = obj["Value"].toVariant();
        r.functionId = QUuid(obj["FunctionId"].toString());
        return r;
    }
};

struct Parameter {
    QString paramName;
    QVariant value;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["ParamName"] = paramName;
        obj["Value"] = QJsonValue::fromVariant(value);
        return obj;
    }
    static Parameter fromJson(const QJsonObject& obj) {
        Parameter p;
        p.paramName = obj["ParamName"].toString();
        p.value = obj["Value"].toVariant();
        return p;
    }
};
struct MethodInfo {
    QUuid clientId;
    QUuid functionId;
    MessageType messageType = MessageType::CallFunction;
    QString methodName;
    QVector<Parameter> params;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["ClientId"] = clientId.toString();
        obj["FunctionId"] = functionId.toString();
        obj["MessageType"] = static_cast<int>(messageType);
        obj["MethodName"] = methodName;
        QJsonArray arr;
        for (const auto& p : params)
            arr.append(p.toJson());
        obj["Params"] = arr;
        return obj;
    }
    static MethodInfo fromJson(const QJsonObject& obj) {
        MethodInfo m;
        m.clientId = QUuid(obj["ClientId"].toString());
        m.functionId = QUuid(obj["FunctionId"].toString());
        m.methodName = obj["MethodName"].toString();
        QJsonArray arr = obj["Params"].toArray();
        for (const auto& v : arr)
            m.params.append(Parameter::fromJson(v.toObject()));
        return m;
    }
};

namespace Protocol {
inline QString getEchoJson() {
    Echo echo;
    return QString(QJsonDocument(echo.toJson()).toJson(QJsonDocument::Compact));
}
inline QString getIdentityJson(const QUuid& id) {
    Identity identity;
    identity.id = id;
    return QString(QJsonDocument(identity.toJson()).toJson(QJsonDocument::Compact));
}
inline QString getMethodInfoJson(const QString& methodName, const QVector<Parameter>& allParams) {
    MethodInfo methodInfo;
    methodInfo.methodName = methodName;
    methodInfo.params = allParams;
    return QString(QJsonDocument(methodInfo.toJson()).toJson(QJsonDocument::Compact));
}
inline MethodInfo getMethodInfo(const QString& methodInfoJson) {
    QJsonDocument doc = QJsonDocument::fromJson(methodInfoJson.toUtf8());
    return MethodInfo::fromJson(doc.object());
}
inline QString getResultJson(const QUuid& functionId, const QVariant& value) {
    Result result;
    result.functionId = functionId;
    result.value = value;
    return QString(QJsonDocument(result.toJson()).toJson(QJsonDocument::Compact));
}
}

#endif // PROTOCOL_H
