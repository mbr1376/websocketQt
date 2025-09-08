#include "clienthub.h"

ClientHub::ClientHub(QObject *parent)
    : Client{parent}
{

}

QString ClientHub::getValue(const QString &name,  qint64 id)
{
    return "Respnose: " + name + "->"+   QString::number(id);
}
