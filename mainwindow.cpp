#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

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

    hotKeyThread = new HotKeyThread();
    connect(hotKeyThread, &HotKeyThread::sendText, this, &MainWindow::receiveShortCut);

    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::listWidgetClicked);

    //hotKeyThread->start();


    Qt::WindowFlags flags;
    flags = Qt::Dialog;
    flags |= Qt::WindowTitleHint;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::CustomizeWindowHint;
    shortcutWindow = new ShortcutWidget();
    shortcutWindow->setWindowFlags(flags);
    connect(shortcutWindow, &ShortcutWidget::updateKeys, this, &MainWindow::updateKeys);


    createMenu();


    readSettings();
    shortcutWindow->setKeys(&hotKeys);

    closeOnTrayIcon = false;

    int size1 = 1010;
    int size2 = 334;

    QList<int> sizes;
    sizes << size1 << size2; // Set the desired sizes in pixels
    ui->splitter_2->setSizes(sizes);

}

void MainWindow::shortcutActivated(QString text)
{
    ui->textEdit->setText(text);
    activate();
}

MainWindow::~MainWindow()
{
    writeSettings();

    if (shortcutWindow != nullptr)
        delete shortcutWindow;

    delete showShortcutAction;

    hotKeyThread->terminate();
    hotKeyThread->wait();
    delete hotKeyThread;
    delete quitAction;
    delete trayIconMenu;
    delete trayIcon;
    delete fileMenu;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (closeOnTrayIcon) {
        if (!event->spontaneous() || !isVisible())
            return;
        if (trayIcon->isVisible()) {
            hide();
            event->ignore();
        }
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

void MainWindow::listWidgetClicked()
{
    if (!connected)
        return;

    QString text = ui->listWidget->currentItem()->text();
    ui->textEdit->setText(text);
    activate();}

void MainWindow::showShortcutDialog()
{
    shortcutWindow->show();
}

void MainWindow::updateKeys(QVector<HotKey *>hotkeys)
{
    if (hotKeyThread != nullptr) {
        hotKeyThread->setStopped(true);
    }

    ui->listWidget->clear();


    HotKey tempKey;
    hotKeys.clear();

    for (int i = 0; i < hotkeys.size(); i++) {
        tempKey.setCode(hotkeys.at(i)->code);
        tempKey.setCtrl(hotkeys.at(i)->ctrl);
        tempKey.setAlt(hotkeys.at(i)->alt);
        tempKey.phrase = hotkeys.at(i)->phrase;
        hotKeys.append(tempKey);
        ui->listWidget->addItem(tempKey.phrase);
    }

    hotKeyThread->setKeys(hotKeys);
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

    fileMenu = menuBar()->addMenu(tr("Shortcuts"));
    fileMenu->addAction(showShortcutAction);
}

void MainWindow::readSettings()
{
    hotKeys.clear();
    QSettings settings("omilo-android", "omilo-android");
    phrases = settings.value("phrases").toStringList();
    codes = settings.value("codes").toStringList();
    ctrls = settings.value("ctrls").toStringList();
    alts = settings.value("alts").toStringList();

    int width = settings.value("width", 800).toInt();
    int height = settings.value("height", 600).toInt();
    int size1 = settings.value("splitter-size1").toInt();
    int size2 = settings.value("spliiter-size2").toInt();

    this->resize(width, height);
    QList<int> sizes;
    sizes << size1 << size2;
    ui->splitter_2->setSizes(sizes);

    HotKey tempKey;

    int size = phrases.size();

    if ( phrases.size() != codes.size())
        return;

    for (int i = 0; i < size; i++) {
        tempKey.phrase = phrases.at(i);
        tempKey.setCode(codes.at(i));
        if (ctrls.at(i) == "true")
            tempKey.setCtrl(true);
        else
            tempKey.setCtrl(false);
        if (alts.at(i) == "true")
            tempKey.setAlt(true);
        else
            tempKey.setAlt(false);
        hotKeys.append(tempKey);
    }


    if (hotKeyThread != nullptr) {
        hotKeyThread->setStopped(true);
    }

    hotKeyThread->setKeys(hotKeys);
    hotKeyThread->start();


    ui->listWidget->addItems(phrases);
}

void MainWindow::writeSettings()
{
    phrases.clear();
    codes.clear();
    ctrls.clear();
    alts.clear();

    for (int i = 0; i < hotKeys.size(); i++) {
        phrases.append(hotKeys.at(i).phrase);
        codes.append(hotKeys.at(i).code);
        if (hotKeys.at(i).ctrl == 0)
            ctrls.append("false");
        else
            ctrls.append("true");
        if (hotKeys.at(i).alt == 0)
            alts.append("false");
        else
            alts.append("true");
    }

    QSettings settings("omilo-android", "omilo-android");

    settings.setValue("phrases", phrases);
    settings.setValue("codes", codes);
    settings.setValue("ctrls", ctrls);
    settings.setValue("alts", alts);
    settings.setValue("width", this->width());
    settings.setValue("height", this->height());
    settings.setValue("splitter-size1", ui->listWidget->size().width());
    settings.setValue("splitter-size2", ui->splitter->size().height());

    qDebug() << ui->listWidget->size().width();
    qDebug() << ui->splitter->size().height();
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
}
