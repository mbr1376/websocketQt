
#include <QApplication>
#include <QTimer>
#include "socket.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "start server";
    Socket server;
    return a.exec();
}
