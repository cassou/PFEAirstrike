#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QDebug>
#include <SDL/SDL.h>

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
    connect(ui->disconnectButton, SIGNAL(released()), this, SLOT(stopPlay()));
    connect(this, SIGNAL(sendKeyEvent(QKeyEvent*,int)), networkManager, SLOT(process_key(QKeyEvent*,int)));
    connect(networkManager, SIGNAL(newPlayerScore(int)), ui->playerScore, SLOT(setNum(int)));
    connect(networkManager, SIGNAL(newHealthPoints(int)), ui->healthPoints, SLOT(setValue(int)));
    connect(networkManager, SIGNAL(newPlayerId(int)), ui->playerIdLabel, SLOT(setNum(int)));

    this->setAttribute(Qt::WA_KeyCompression,true);
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
    networkManager->setIP(ui->ipEdit->text(),ui->portEdit->text().toInt());
    networkManager->moveToThread(networkThread);
    connect(networkThread, SIGNAL(started()), networkManager, SLOT(network_init()));
    networkThread->start();
    startPlay();
}

void MainWindow::keyPressEvent(QKeyEvent * event){

    if(!event->isAutoRepeat()){
        emit networkManager->process_key(event, 1);
this->displayText("Key pressed " + QString::number(event->count()));
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent * event){

    if(!event->isAutoRepeat()){
        emit sendKeyEvent(event, -1);
       this->displayText("Key released "+ QString::number(event->key()));
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
SDL_Event event;

