#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectButton,SIGNAL(clicked()),this,SLOT(connect_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connect_clicked(){
    ui->debugTextEdit->appendPlainText("Coucou");

}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if (event->key() == Qt::Key_Escape && !event->isAutoRepeat()) {
            ui->debugTextEdit->appendPlainText("Coucou espechapace");
        } else {
            QWidget::keyPressEvent(event);
        }

}

void MainWindow::keyReleaseEvent(QKeyEvent * event){
    if (event->key() == Qt::Key_Escape && !event->isAutoRepeat()) {
            ui->debugTextEdit->appendPlainText("Coucou espechapace release");
        } else {
            QWidget::keyPressEvent(event);
        }

}
