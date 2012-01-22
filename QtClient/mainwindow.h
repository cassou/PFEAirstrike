#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkmanager.h"
#include <QTimer>
#include <QSound>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void writeSettings();
    void readSettings();
    void closeEvent(QCloseEvent *event);

public slots:
    void displayText(QString string);
    void stopPlay();

private:
    Ui::MainWindow *ui;
    NetworkManager *networkManager;
    QThread *networkThread;
    bool isBot;
    QTimer* botTimer;
    bool started;
    QSound* bullet;

private slots:
    void connect_clicked();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void startPlay();
    void setBot(int state);
    void setSprite(int idInTeam);

signals:
    void sendKeyEvent(QKeyEvent * event, int key_status);
    void setIP(QString ip_addr, int port);
    void setRequestedTeam(int team);
    void startNetworkManager();
};

#endif // MAINWINDOW_H
