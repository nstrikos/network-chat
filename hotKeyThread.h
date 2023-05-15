#ifndef HOTKEYTHREAD_H
#define HOTKEYTHREAD_H

#include <QThread>
#include "hotKey.h"

class HotKeyThread : public QThread
{
    Q_OBJECT
public:
    HotKeyThread();
    void stop();
    void setStopped(bool stopped);

protected:
    void run();

private:
    volatile bool stopped;
    QVector<HotKey> hotKeys;

signals:
    void playPressed();
    void stopPressed();
    void showWindowPressed();
    void pausePressed();
};

#endif // HOTKEYTHREAD_H
