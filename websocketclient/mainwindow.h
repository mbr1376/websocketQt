#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "websocketclient.h"
#include "decrypthelper.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
/**
 * @brief class mainwindow
 * 
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Main Window object
     * 
     * @param parent 
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btnConnect_clicked();

    void on_btnDisconnected_clicked();

    void on_btnSend_clicked();

private:
    DecryptHelper dec;
    Ui::MainWindow *ui;
    WebSocketClient* websocketClient;
    void onReceivedMessage(QVariantList paramms);
};
#endif // MAINWINDOW_H
