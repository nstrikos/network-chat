HEADERS       = chatdialog.h \
                client.h \
                clientsocket.h \
                connection.h \
                mythread.h \
                peermanager.h \
                server.h \
                tripserver.h
SOURCES       = chatdialog.cpp \
                client.cpp \
                clientsocket.cpp \
                connection.cpp \
                main.cpp \
                mythread.cpp \
                peermanager.cpp \
                server.cpp \
                tripserver.cpp
FORMS         = chatdialog.ui
QT           += network widgets
requires(qtConfig(udpsocket))
requires(qtConfig(listwidget))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/network-chat
INSTALLS += target


