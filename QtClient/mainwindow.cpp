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
    if(!event->isAutoRepeat()){
        networkManager->process_key(event, 1);
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent * event){
    if(!event->isAutoRepeat()){
        networkManager->process_key(event, 0);
    } else {
        QWidget::keyPressEvent(event);
    }
}
