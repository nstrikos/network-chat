#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    peers = 0;
    statusBar()->showMessage("Not connected");

    ui->textEdit->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newParticipant(QString text)
{
    peers++;
    qDebug()<< text << ": connected";
    checkButton();
}

void MainWindow::participantLeft(QString text)
{
    peers--;
    qDebug() << text << ": disconnected";
    checkButton();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->textEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return) {
            // Special tab handling
            qDebug("Enter Key Pressed...");
            activate();
            return true;
        } else {
            return QMainWindow::eventFilter(object, event);
        }
    } else {
        return QMainWindow::eventFilter(object, event);
    }
}

void MainWindow::on_pushButton_clicked()
{
    activate();
}


void MainWindow::on_textEdit_2_textChanged()
{
    if (ui->textEdit_2->document()->toPlainText() != "")
        ui->pushButton_2->setEnabled(true);
}

void MainWindow::checkButton()
{
    if (ui->textEdit->document()->toPlainText() != "" && peers > 0)
        ui->pushButton->setEnabled(true);
    else
        ui->pushButton->setEnabled(false);

    if (peers > 0)
        statusBar()->showMessage("Connected");
    else
        statusBar()->showMessage("Not connected");
}


void MainWindow::on_textEdit_textChanged()
{
    checkButton();
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit_2->clear();
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::activate()
{
    QString text = ui->textEdit->document()->toPlainText();
    emit sendText(text);
    ui->textEdit->clear();

    QTextCursor cursor = QTextCursor(ui->textEdit_2->document());
    cursor.setPosition(0);
    ui->textEdit_2->setTextCursor(cursor);
    ui->textEdit_2->insertPlainText(text + "\n");
}

