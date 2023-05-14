HEADERS       = chatdialog.h \
                client.h \
                clientsocket.h \
                connection.h \
                hotKeyThread.h \
                mainwindow.h \
                mythread.h \
                peermanager.h \
                server.h \
                tripserver.h
SOURCES       = chatdialog.cpp \
                client.cpp \
                clientsocket.cpp \
                connection.cpp \
                hotKeyThread.cpp \
                main.cpp \
                mainwindow.cpp \
                mythread.cpp \
                peermanager.cpp \
                server.cpp \
                tripserver.cpp
FORMS         = chatdialog.ui \
    mainwindow.ui
QT           += network widgets
requires(qtConfig(udpsocket))
requires(qtConfig(listwidget))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/network-chat
INSTALLS += target
LIBS += -lX11



