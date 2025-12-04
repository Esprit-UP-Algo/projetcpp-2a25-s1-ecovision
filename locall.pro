QT += core gui widgets sql

CONFIG += c++17
TEMPLATE = app
TARGET = locall

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    local.cpp

HEADERS += \
    mainwindow.h \
    connection.h \
    local.h

FORMS += \
    mainwindow.ui
