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
    void on_pushButton_clicked();

    void on_textEdit_2_textChanged();

    void on_textEdit_textChanged();

    void on_pushButton_2_clicked();


private:
    Ui::MainWindow *ui;
    unsigned int peers;

    void activate();

    void checkButton();
};

#endif // MAINWINDOW_H
