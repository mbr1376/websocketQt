#ifndef CLIENTHUB_H
#define CLIENTHUB_H


#include <QTimer>

#include "client.h"
#include "protocol.h"
#include "encrypthelper.h"
class ClientHub : public Client
{
    Q_OBJECT
public:
    explicit ClientHub(QObject *parent = nullptr);

    QString getValue(const QString &name, qint64 id);
    void startRealTimeData();
signals:
    void sendDataRealTime (const QString &metodName,const QVector<Parameter> &params);

private:
    QTimer *timer;
    int currentData{0};
    EncryptHelper enc;
};

#endif // CLIENTHUB_H
