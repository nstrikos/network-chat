#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void newParticipant(QString text);
    void participantLeft(QString text);

protected:
    bool eventFilter(QObject *object, QEvent *event);

signals:
    void sendText(QString text);

private slots:
    void historyEditChanged();
    void talkButtonClicked();
    void clearButtonClicked();
    void textEditChanged();

private:
    Ui::MainWindow *ui;
    unsigned int peers;
    bool connected;

    void activate();

    void checkButton();
};

#endif // MAINWINDOW_H
