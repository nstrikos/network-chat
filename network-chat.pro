HEADERS       = chatdialog.h \
                client.h \
                clientsocket.h \
                connection.h \
                hotKey.h \
                hotKeyThread.h \
                mainwindow.h \
                myCombobox.h \
                myPushbutton.h \
                mythread.h \
                peermanager.h \
                server.h \
                shortcutWidget.h \
                tripserver.h
SOURCES       = chatdialog.cpp \
                client.cpp \
                clientsocket.cpp \
                connection.cpp \
                hotKey.cpp \
                hotKeyThread.cpp \
                main.cpp \
                mainwindow.cpp \
                myCombobox.cpp \
                myPushbutton.cpp \
                mythread.cpp \
                peermanager.cpp \
                server.cpp \
                shortcutWidget.cpp \
                tripserver.cpp
FORMS         = chatdialog.ui \
    mainwindow.ui \
    shortcutWidget.ui
QT           += network widgets
requires(qtConfig(udpsocket))
requires(qtConfig(listwidget))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/network-chat
INSTALLS += target
LIBS += -lX11



