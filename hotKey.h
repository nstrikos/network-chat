#ifndef HOTKEY_H
#define HOTKEY_H

#include <QString>

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

private:
    QString code;
    unsigned int alt;
    unsigned int ctrl;
    void updateKeyCode();
    void updateModifiers();
};

#endif // HOTKEY_H
