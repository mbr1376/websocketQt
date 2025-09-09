#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "websocketclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();

    void on_btnDisconnected_clicked();

    void on_btnSend_clicked();

private:
    Ui::MainWindow *ui;
    WebSocketClient* websocketClient;
    void onReceivedMessage(QVariantList paramms);
};
#endif // MAINWINDOW_H
