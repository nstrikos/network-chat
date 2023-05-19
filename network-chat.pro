HEADERS       = chatdialog.h \
                client.h \
                clientsocket.h \
                connection.h \
                hotKey.h \
                hotKeyThread.h \
                mainwindow.h \
                myCombobox.h \
                mythread.h \
                peermanager.h \
                server.h \
                shortcutDialog.h \
                shortcutWidget.h \
                shortcutWindow.h \
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
                mythread.cpp \
                peermanager.cpp \
                server.cpp \
                shortcutDialog.cpp \
                shortcutWidget.cpp \
                shortcutWindow.cpp \
                tripserver.cpp
FORMS         = chatdialog.ui \
    mainwindow.ui \
    shortcutDialog.ui \
    shortcutWidget.ui \
    shortcutWindow.ui
QT           += network widgets
requires(qtConfig(udpsocket))
requires(qtConfig(listwidget))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/network-chat
INSTALLS += target
LIBS += -lX11



