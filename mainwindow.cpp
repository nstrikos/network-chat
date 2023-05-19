#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connected = false;
    disableControls();

    ui->clearButton->setEnabled(false);

    ui->textEdit->installEventFilter(this);

    connect(ui->historyEdit, &QTextEdit::textChanged, this, &MainWindow::historyEditChanged);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::textEditChanged);
    connect(ui->talkButton, &QPushButton::clicked, this, &MainWindow::talkButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearButtonClicked);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::showWindow);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);

    HotKey tempKey;
    QVector<HotKey>hotKeys;


    tempKey.setCode("8");
    tempKey.setCtrl(true);
    tempKey.setAlt(false);
    tempKey.phrase = "ok let's do it";
    hotKeys.append(tempKey);

    hotKeyThread = new HotKeyThread(hotKeys);
    connect(hotKeyThread, &HotKeyThread::sendText, this, &MainWindow::receiveShortCut);

    hotKeyThread->start();

    shortcutWindow = nullptr;

    createMenu();
}

void MainWindow::shortcutActivated(QString text)
{
    qDebug() << "Got here!";
    //    if (!connected)
    //        return;
    ui->textEdit->setText(text);
    activate();
}

MainWindow::~MainWindow()
{
    if (shortcutWindow != nullptr)
        delete shortcutWindow;

    delete showShortcutAction;

    hotKeyThread->terminate();
    hotKeyThread->wait();
    delete hotKeyThread;
    delete quitAction;
    delete trayIconMenu;
    delete trayIcon;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!event->spontaneous() || !isVisible())
        return;
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void MainWindow::clientConnected()
{
    connected = true;
    enableControls();
}

void MainWindow::clientDisconnected()
{
    connected = false;
    disableControls();
}

void MainWindow::receiveShortCut(QString text)
{
    if (!connected)
        return;
    ui->textEdit->setText(text);
    activate();
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

void MainWindow::showWindow()
{
    this->show();    
}

void MainWindow::showShortcutDialog()
{
    if (shortcutWindow == nullptr) {
        shortcutWindow = new ShortcutWidget();
        connect(shortcutWindow, &ShortcutWidget::updateKeys, this, &MainWindow::updateKeys);
    }

    shortcutWindow->show();
}

void MainWindow::updateKeys(QVector<HotKey *>hotkeys)
{



    if (hotKeyThread != nullptr) {
        hotKeyThread->setStopped(true);
        //hotKeyThread->terminate();
        //hotKeyThread->wait();
        //delete hotKeyThread;
    }


    HotKey tempKey;
    QVector<HotKey>hotKeys;


//    tempKey.setCode("8");
//    tempKey.setCtrl(false);
//    tempKey.setAlt(true);
//    tempKey.phrase = "ok let's do it";
//    hotKeys.append(tempKey);

    for (int i = 0; i < hotkeys.size(); i++) {
        qDebug() << hotkeys.at(i)->code;
            tempKey.setCode(hotkeys.at(i)->code);
            tempKey.setCtrl(hotkeys.at(i)->ctrl);
            tempKey.setAlt(hotkeys.at(i)->alt);
            tempKey.phrase = hotkeys.at(i)->phrase;
            hotKeys.append(tempKey);
    }

    hotKeyThread->setKeys(hotKeys);


    //        hotKeyThread = new HotKeyThread(hotKeys);
    //        connect(hotKeyThread, &HotKeyThread::sendText, this, &MainWindow::receiveShortCut);

    hotKeyThread->start();
}

void MainWindow::checkButton()
{
    if (!connected)
        return;

    if (ui->textEdit->document()->toPlainText() != "")
        ui->talkButton->setEnabled(true);
}

void MainWindow::enableControls()
{
    statusBar()->showMessage("Connected");
    ui->textEdit->setEnabled(true);
    ui->textEdit->setFocus();
}

void MainWindow::disableControls()
{
    ui->textEdit->setEnabled(false);
    ui->talkButton->setEnabled(false);
    statusBar()->showMessage("Not connected");
}

void MainWindow::createMenu()
{
    showShortcutAction = new QAction(tr("Edit shortcuts..."), this);


    connect(showShortcutAction, &QAction::triggered, this, &MainWindow::showShortcutDialog);

    QMenu* fileMenu = menuBar()->addMenu(tr("Shortcuts"));
    fileMenu->addAction(showShortcutAction);
}

void MainWindow::activate()
{
    if (!connected)
        return;

    QString text = ui->textEdit->document()->toPlainText();
    emit sendText(text);    
    ui->textEdit->clear();
    ui->talkButton->setEnabled(false);

    QTextCursor cursor = QTextCursor(ui->historyEdit->document());
    cursor.setPosition(0);
    ui->historyEdit->setTextCursor(cursor);
    ui->historyEdit->insertPlainText(text + "\n");

    ui->textEdit->setFocus();


    if (text == "1") {
        if (hotKeyThread != nullptr) {
            hotKeyThread->setStopped(true);
            //hotKeyThread->terminate();
            //hotKeyThread->wait();
            //delete hotKeyThread;
        }


        HotKey tempKey;
        QVector<HotKey>hotKeys;


        tempKey.setCode("8");
        tempKey.setCtrl(false);
        tempKey.setAlt(true);
        tempKey.phrase = "ok let's do it";
        hotKeys.append(tempKey);

        hotKeyThread->setKeys(hotKeys);


        //        hotKeyThread = new HotKeyThread(hotKeys);
        //        connect(hotKeyThread, &HotKeyThread::sendText, this, &MainWindow::receiveShortCut);

        hotKeyThread->start();

    } else if (text == "2") {

        if (hotKeyThread != nullptr) {
            hotKeyThread->setStopped(true);
            //            hotKeyThread->terminate();
            //            hotKeyThread->wait();
            //            delete hotKeyThread;
        }


        HotKey tempKey;
        QVector<HotKey>hotKeys;


        tempKey.setCode("9");
        tempKey.setCtrl(true);
        tempKey.setAlt(false);
        tempKey.phrase = "ok";

        hotKeys.append(tempKey);
        //        hotKeyThread = new HotKeyThread(hotKeys);

        //        connect(hotKeyThread, &HotKeyThread::sendText, this, &MainWindow::receiveShortCut);

        hotKeyThread->setKeys(hotKeys);

        hotKeyThread->start();
    }
}
