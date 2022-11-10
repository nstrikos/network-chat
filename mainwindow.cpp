#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    peers = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newParticipant(QString text)
{
    peers++;
    qDebug()<< text << ": connected";
    if (peers > 0)
        ui->pushButton->setEnabled(true);
}

void MainWindow::participantLeft(QString text)
{
    peers--;
    qDebug() << text << ": disconnected";
    if (peers <= 0)
        ui->pushButton->setEnabled(false);
}

void MainWindow::on_pushButton_clicked()
{
    emit sendText(ui->lineEdit->text());
}

