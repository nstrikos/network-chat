#include <QApplication>

//#include "chatdialog.h"
#include "mainwindow.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //ChatDialog dialog;
    //dialog.show();

    MainWindow mainWindow;
    mainWindow.show();

    Client client;

    QObject::connect(&mainWindow, &MainWindow::sendText, &client, &Client::sendMessage);
    QObject::connect(&client, &Client::newParticipant, &mainWindow, &MainWindow::newParticipant);
    QObject::connect(&client, &Client::participantLeft, &mainWindow, &MainWindow::participantLeft);

    return app.exec();
}
