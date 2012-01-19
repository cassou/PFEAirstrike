#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QDebug>
#include <SDL/SDL.h>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{

    isBot=false;

    ui->setupUi(this);
    readSettings();
    displayText("Start");

    networkThread = new QThread();
    networkManager = new NetworkManager();

    connect(ui->connectButton,SIGNAL(clicked()),this,SLOT(connect_clicked()));
    connect(networkManager, SIGNAL(writeText(QString)), this, SLOT(displayText(QString)));
    connect(ui->disconnectButton, SIGNAL(released()), this, SLOT(stopPlay()));
    connect(this, SIGNAL(sendKeyEvent(QKeyEvent*,int)), networkManager, SLOT(process_key(QKeyEvent*,int)));
    connect(networkManager, SIGNAL(newPlayerScore(int)), ui->playerScore, SLOT(setNum(int)));
    connect(networkManager, SIGNAL(newHealthPoints(int)), ui->healthPoints, SLOT(setValue(int)));
    connect(networkManager, SIGNAL(newPlayerId(int)), ui->playerIdLabel, SLOT(setNum(int)));
    connect(ui->checkBoxBot, SIGNAL(clicked(bool)), this, SLOT(setBot(bool)));
    connect(ui->nameEdit, SIGNAL(textChanged(QString)), networkManager, SLOT(setLogin(QString)));
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
        //this->displayText("Key pressed " + QString::number(event->count()));
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent * event){
    if(event->key() == Qt::Key_Escape)
    {
        stopPlay();
        return;
    }
    if(!event->isAutoRepeat()){
        emit sendKeyEvent(event, -1);
       //this->displayText("Key released "+ QString::number(event->key()));
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::startPlay()
{
    if(!isBot)
    {
        this->grabKeyboard();
    }
    else
    {
       botTimer = new QTimer();
       botTimer->setInterval(100);
       connect(botTimer, SIGNAL(timeout()), networkManager, SLOT(set_rand_key()));
       botTimer->start();
    }
}

void MainWindow::stopPlay()
{
    if(isBot)
    {
        botTimer->stop();
    }
    else
    {
        this->releaseKeyboard();

    }
}

void MainWindow::setBot(bool state)
{
    isBot = state;
}

void MainWindow::writeSettings()
{
    QSettings settings("airstrike_client.conf",QSettings::IniFormat);

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();

    settings.beginGroup("Values");
    settings.setValue("ip", ui->ipEdit->text());
    settings.setValue("port", ui->portEdit->text());
    settings.setValue("login", ui->nameEdit->text());
    settings.setValue("bot", ui->checkBoxBot->isChecked());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings("airstrike_client.conf",QSettings::IniFormat);

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();

    settings.beginGroup("Values");
    ui->ipEdit->setText(settings.value("ip").toString());
    ui->portEdit->setText(settings.value("port").toString());
    ui->nameEdit->setText(settings.value("login").toString());
    ui->checkBoxBot->setChecked(settings.value("bot").toBool());
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
        writeSettings();
        event->accept();
}
