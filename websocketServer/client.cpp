#include "client.h"

Client::Client(QObject *parent)
    : QObject{parent}
{}

void Client::invoke(const QString &methodName, const QVector<Parameter> &allParams)
{

}
