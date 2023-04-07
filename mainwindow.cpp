#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->talkButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    peers = 0;
    connected = false;
    statusBar()->showMessage("Not connected");

    ui->textEdit->installEventFilter(this);

    connect(ui->historyEdit, &QTextEdit::textChanged, this, &MainWindow::historyEditChanged);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::textEditChanged);
    connect(ui->talkButton, &QPushButton::clicked, this, &MainWindow::talkButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newParticipant(QString text)
{
    peers++;
    connected = true;
    qDebug()<< text << ": connected";
    checkButton();
}

void MainWindow::participantLeft(QString text)
{
    peers--;
    if (peers <= 0)
        connected = false;
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

void MainWindow::historyEditChanged()
{
    if (ui->historyEdit->document()->toPlainText() != "")
        ui->clearButton->setEnabled(true);
}

void MainWindow::talkButtonClicked()
{
    activate();
}

void MainWindow::clearButtonClicked()
{
    ui->historyEdit->clear();
    ui->clearButton->setEnabled(false);
}

void MainWindow::textEditChanged()
{
    checkButton();
}

void MainWindow::checkButton()
{
    if (ui->textEdit->document()->toPlainText() != "" && connected)
        ui->talkButton->setEnabled(true);

    if (ui->historyEdit->document()->toPlainText() != "")
        ui->talkButton->setEnabled(true);

    if (connected)
        statusBar()->showMessage("Connected");
    else
        statusBar()->showMessage("Not connected");
}

void MainWindow::activate()
{
    if (!connected)
        return;

    QString text = ui->textEdit->document()->toPlainText();
    emit sendText(text);
    ui->textEdit->clear();

    QTextCursor cursor = QTextCursor(ui->historyEdit->document());
    cursor.setPosition(0);
    ui->historyEdit->setTextCursor(cursor);
    ui->historyEdit->insertPlainText(text + "\n");
}

