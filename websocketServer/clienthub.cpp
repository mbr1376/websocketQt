#include "clienthub.h"

ClientHub::ClientHub(QObject *parent)
    : Client{parent}
{

}

QString ClientHub::getValue(qint64 id)
{
    return "respnose" + QString::number(id);
}
