#include "hotKeyThread.h"

#include <QDebug>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

HotKeyThread::HotKeyThread()
{
    qDebug() << "Registering hot keys...";
    stopped = false;

    HotKey tempKey;

    tempKey.setCode("F11");
    tempKey.setCtrl(true);
    tempKey.setAlt(false);

    hotKeys.append(tempKey);
    qDebug() << tempKey.keycode << tempKey.modifiers;

    Display *dpy = XOpenDisplay(0);

    tempKey.keycode = XKeysymToKeycode(dpy, XK_B);
    tempKey.modifiers = ControlMask;

    hotKeys.append(tempKey);
    qDebug() << tempKey.keycode << tempKey.modifiers;
}

void HotKeyThread::stop()
{
    stopped = true;
}

void HotKeyThread::setStopped(bool stopped)
{
    this->stopped = stopped;
}

void HotKeyThread::run()
{
    Display    *dpy     =  XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    XEvent      ev;
    KeySym key;
    char text[255];

    Window          grab_window     = root;
    Bool            owner_events    = False;
    int             pointer_mode    = GrabModeAsync;
    int             keyboard_mode   = GrabModeAsync;

    for (int i = 0; i < hotKeys.size(); i++) {
        XGrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers, grab_window, owner_events,
                 pointer_mode,
                 keyboard_mode);
        XGrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask, grab_window, owner_events,
                 pointer_mode,
                 keyboard_mode);
        XGrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | Mod2Mask, grab_window, owner_events,
                 pointer_mode,
                 keyboard_mode);
        XGrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask | Mod2Mask, grab_window, owner_events,
                 pointer_mode,
                 keyboard_mode);
    }

    XSelectInput(dpy, root, KeyPressMask);
    while(!stopped)
    {
        XNextEvent(dpy, &ev);
        switch(ev.type)
        {
        case KeyPress:
            XLookupString(&ev.xkey,text,255,&key,0);
            qDebug() << key << XK_V;

            for (int i = 0; i < hotKeys.size(); i++) {
                if (key == hotKeys.at(i).keychar) {
                    qDebug() << hotKeys.at(i).keychar << " pressed";
                } else if ( key == hotKeys.at(i).keychar2) {
                    qDebug() << hotKeys.at(i).keychar2 << " pressed";
                }
            }
        }
    }

    if (stopped)
    {
//        XUngrabKey(dpy,keycode,modifiers,grab_window);
//        XUngrabKey(dpy,keycode2,modifiers,grab_window);

        for (int i = 0; i < hotKeys.size(); i++) {
            XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers, grab_window);
            XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask, grab_window);
            XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | Mod2Mask, grab_window);
            XUngrabKey(dpy, hotKeys.at(i).keycode, hotKeys.at(i).modifiers | LockMask | Mod2Mask, grab_window);
        }
    }
    stopped = true;
    XCloseDisplay(dpy);
}
