QT += core gui widgets sql charts network

CONFIG += c++17

TEMPLATE = app
TARGET = locall

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    local.cpp \
    notificationwidget.cpp \
    reclamation.cpp \
    smsmanager.cpp

HEADERS += \
    mainwindow.h \
    connection.h \
    local.h \
    notificationwidget.h \
    reclamation.h \
    smsmanager.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
