#ifndef HOTKEY_H
#define HOTKEY_H

#include <QString>

class MainWindow;

class HotKey
{
public:
    HotKey();
    void setCode(QString shortcut);
    void setAlt(bool alt);
    void setCtrl(bool ctrl);
    int keycode;
    int keychar;
    int keychar2;
    unsigned int modifiers;
    QString phrase;

private:
    QString code;
    unsigned int alt;
    unsigned int ctrl;
    void updateKeyCode();
    void updateModifiers();
    MainWindow *mainWindow;
};

#endif // HOTKEY_H
