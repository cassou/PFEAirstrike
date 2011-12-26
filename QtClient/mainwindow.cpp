#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectButton,SIGNAL(clicked()),this,SLOT(connect_clicked()));
    displayText("Start");
    networkThread = new QThread();

    networkManager = new NetworkManager();

    connect(networkManager, SIGNAL(writeText(QString)), this, SLOT(displayText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayText(QString string){
    ui->debugTextEdit->appendPlainText(string);
}

void MainWindow::connect_clicked(){
    ui->debugTextEdit->appendPlainText("Coucou");
    this->grabKeyboard();

    //networkThread->start();
    //networkManager->moveToThread(networkThread);

    networkManager->network_init();
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if (event->key() == Qt::Key_Escape && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Echap");
    } else {
        QWidget::keyPressEvent(event);
    }
    if (event->key() == Qt::Key_Right && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Right");
    } else {
        QWidget::keyPressEvent(event);
    }
    if (event->key() == Qt::Key_Left && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Left");
    } else {
        QWidget::keyPressEvent(event);
    }
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Space");
    } else {
        QWidget::keyPressEvent(event);
    }
    if (event->key() == Qt::Key_Control && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Ctrl");
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent * event){
    if (event->key() == Qt::Key_Escape && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Echap release");
    } else {
        QWidget::keyPressEvent(event);
    }
    if (event->key() == Qt::Key_Right && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Right release");
    } else {
        QWidget::keyPressEvent(event);
    }
    if (event->key() == Qt::Key_Left && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Left release");
    } else {
        QWidget::keyPressEvent(event);
    }
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Space release");
    } else {
        QWidget::keyPressEvent(event);
    }
    if (event->key() == Qt::Key_Control && !event->isAutoRepeat()) {
        ui->debugTextEdit->appendPlainText("Ctrl release");
    } else {
        QWidget::keyPressEvent(event);
    }
}
