#include "hotKeyThread.h"

#include <QDebug>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

HotKeyThread::HotKeyThread()
{
    qDebug() << "Registering hot keys...";
    stopped = false;

    HotKey tempKey;
    Display *dpy = XOpenDisplay(0);
    tempKey.keycode = XKeysymToKeycode(dpy, XK_A);
    tempKey.modifiers = ControlMask | Mod1Mask;

    hotKeys.append(tempKey);

    tempKey.keycode = XKeysymToKeycode(dpy, XK_F11);
    tempKey.modifiers = Mod2Mask;

    hotKeys.append(tempKey);
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

    unsigned int    modifiers = ControlMask | Mod1Mask;
    unsigned int    modifiers2 = Mod2Mask;
    int             keycode         = XKeysymToKeycode(dpy, XK_V);
    int             keycode2        = XKeysymToKeycode(dpy, XK_F11);
    int             keycode3        = XKeysymToKeycode(dpy, XK_F12);
    int             keycode4        = XKeysymToKeycode(dpy, XK_F9);
    Window          grab_window     = root;
    Bool            owner_events    = False;
    int             pointer_mode    = GrabModeAsync;
    int             keyboard_mode   = GrabModeAsync;

    //    XGrabKey(dpy, keycode, modifiers, grab_window, owner_events,
    //             pointer_mode,
    //             keyboard_mode);

    //    XGrabKey(dpy, keycode, modifiers2, grab_window, owner_events,
    //             pointer_mode,
    //             keyboard_mode);

    //    XGrabKey(dpy, keycode2, modifiers, grab_window, owner_events,
    //             pointer_mode,
    //             keyboard_mode);
    //    XGrabKey(dpy, keycode3, modifiers, grab_window, owner_events,
    //             pointer_mode,
    //             keyboard_mode);
    //    XGrabKey(dpy, keycode4, modifiers, grab_window, owner_events,
    //             pointer_mode,
    //             keyboard_mode);

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
            if (key == XK_A)
            {
                //F10 pressed
                qDebug() << "A pressed...";
                emit playPressed();
            }
            if (key == XK_a)
            {
                //F10 pressed
                qDebug() << "a pressed...";
                emit playPressed();
            }
            if (key == 65470)
            {
                //F10 pressed
                qDebug() << "F10 pressed...";
                emit playPressed();
            }
            else if (key == 65480)
            {
                //F11 pressed
                qDebug() << "F11 pressed...";
                emit stopPressed();
            }
            //else if (key == 65481)
            else if (key == 65481)
            {
                qDebug() << "F12 pressed...";
                emit showWindowPressed();
            }
            else if (key == 65478)
            {
                //F9 pressed
                qDebug() << "F9 pressed...";
                emit pausePressed();
            }
        }
    }

    if (stopped)
    {
        XUngrabKey(dpy,keycode,modifiers,grab_window);
        XUngrabKey(dpy,keycode2,modifiers,grab_window);
    }
    stopped = true;
    XCloseDisplay(dpy);
}
