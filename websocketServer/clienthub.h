#ifndef CLIENTHUB_H
#define CLIENTHUB_H

#include "client.h"

class ClientHub : public Client
{
    Q_OBJECT
public:
    explicit ClientHub(QObject *parent = nullptr);

    QString getValue(const QString &name, qint64 id);

signals:
};

#endif // CLIENTHUB_H
