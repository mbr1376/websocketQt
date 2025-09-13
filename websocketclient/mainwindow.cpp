#include "mainwindow.h"
#include "./ui_mainwindow.h"

/**
 * @brief Construct a new Main Window:: Main Window object
 * 
 * @param parent 
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    websocketClient = new WebSocketClient(QUrl(QStringLiteral("ws://127.0.01:12345")));

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete websocketClient;
}
void MainWindow::on_btnConnect_clicked()
{
    auto connected = websocketClient->Connect();
    ui->btnConnect->setEnabled(!connected);
    ui->btnDisconnected->setEnabled(connected);
    if (connected){
        ui->lbStatus->setText("Websocket is connected!");
        websocketClient->On("realTimeData",[this](QVariantList params){
            onReceivedMessage(params);
        });
    }

}

void MainWindow::onReceivedMessage(QVariantList paramms)
{
    foreach (auto param, paramms) {
        ui->textBrowser->append("Parameter is :" + param.value<Parameter>().value.toString());
    }
}

void MainWindow::on_btnDisconnected_clicked()
{
    auto disconnected = websocketClient->Disconnect();
    ui->btnConnect->setEnabled(disconnected);
    ui->btnDisconnected->setEnabled(!disconnected);
    ui->lbStatus->setText("Websocket is Disonnected!");
}

void MainWindow::on_btnSend_clicked()
{
    Parameter p1;
//    p1.paramName ="Message";
//    p1.value = ui->textEdit->toPlainText();
//    websocketClient->Invoke("SendMessage",QVector<Parameter>() << p1,[=](const QVariant &result){
//        ui->lbResult->setText(result.toString()+"\n");
//        qDebug() << result;
//    });
    p1.paramName ="value";
    p1.value = ui->lineEdit->text();
    websocketClient->Invoke("getData",QVector<Parameter>() << p1,[=](const QVariant &result){
        ui->lbResult->setText(result.toString()+"\n");

    });

}
