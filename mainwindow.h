#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMenu>
#include "hotKeyThread.h"
#include "shortcutWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void shortcutActivated(QString text);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void clientConnected();
    void clientDisconnected();
    void receiveShortCut(QString text);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

signals:
    void sendText(QString text);

private slots:
    void historyEditChanged();
    void talkButtonClicked();
    void clearButtonClicked();
    void textEditChanged();
    void showWindow();

    void showShortcutDialog();
    void updateKeys(QVector<HotKey*>hotkeys);

private:
    Ui::MainWindow *ui;
    bool connected;
    QSystemTrayIcon *trayIcon;
    QAction *quitAction;
    QMenu *trayIconMenu;

    void activate();
    void checkButton();
    void enableControls();
    void disableControls();

    HotKeyThread *hotKeyThread;
    ShortcutWidget *shortcutWindow;

    QMenu* fileMenu;

    QAction* showShortcutAction;

    void createMenu();

    bool closeOnTrayIcon;
};

#endif // MAINWINDOW_H
