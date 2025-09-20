#include "clienthub.h"

ClientHub::ClientHub(QObject *parent)
    : Client{parent}
{

}

QString ClientHub::getValue(const QString &name,  qint64 id)
{
    return "Respnose: " + name + "->"+   QString::number(id);
}

void ClientHub::startRealTimeData()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this](){
        currentData++;
        QVector<Parameter> params;
        params.append(Parameter{enc.encrypt("paramsname"),enc.encrypt("Data")});
        params.append(Parameter{enc.encrypt("value"), enc.encrypt(QString::number(currentData))});
        emit sendDataRealTime("realTimeData",params);
    });

    timer->start(1000);
}
