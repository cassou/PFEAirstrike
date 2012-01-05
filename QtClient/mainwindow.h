#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkmanager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void displayText(QString string);

private:
    Ui::MainWindow *ui;
    NetworkManager *networkManager;
    QThread *networkThread;

private slots:
    void connect_clicked();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void startPlay();
    void stopPlay();

signals:
    void sendKeyEvent(QKeyEvent * event, int key_status);
};

#endif // MAINWINDOW_H
