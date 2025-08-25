#include "socket.h"

Socket::Socket(QObject *parent)
    : QObject{parent}
{}

void Socket::onNewConnection()
{

}

void Socket::onClientDisconnected()
{

}

void Socket::onTextMessageReceived(const QString &message)
{

}
