#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

signals:
    void sendText(QString text);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    unsigned int peers;
};

#endif // MAINWINDOW_H
