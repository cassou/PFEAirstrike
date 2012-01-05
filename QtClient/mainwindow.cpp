#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QDebug>

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
    qDebug() << string;
}

void MainWindow::connect_clicked(){
    networkManager->moveToThread(networkThread);
    networkThread->start();
    networkManager->network_init(ui->ipEdit->text(), ui->portEdit->text().toInt());

}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if(!event->isAutoRepeat()){
        networkManager->process_key(event, 1);
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent * event){
    if(!event->isAutoRepeat()){
        networkManager->process_key(event, -1);
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::startPlay()
{
    this->grabKeyboard();
}

void MainWindow::stopPlay()
{
    this->releaseKeyboard();
}
