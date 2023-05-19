#ifndef HOTKEYTHREAD_H
#define HOTKEYTHREAD_H

#include <QThread>
#include "hotKey.h"

class HotKeyThread : public QThread
{
    Q_OBJECT
public:
    HotKeyThread(QVector<HotKey> keys);
    ~HotKeyThread();
    void setStopped(bool stopped);
    void setKeys(QVector<HotKey> keys);

signals:
    void sendText(QString text);

protected:
    void run();

private:    
    QVector<HotKey> hotKeys;
    bool stopped = false;
};

#endif // HOTKEYTHREAD_H
