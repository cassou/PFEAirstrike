#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkmanager.h"
#include <QTimer>

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

private:
    Ui::MainWindow *ui;
    NetworkManager *networkManager;
    QThread *networkThread;
    bool isBot;
    QTimer* botTimer;

private slots:
    void connect_clicked();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void startPlay();
    void stopPlay();
    void setBot(bool state);

signals:
    void sendKeyEvent(QKeyEvent * event, int key_status);
};

#endif // MAINWINDOW_H
