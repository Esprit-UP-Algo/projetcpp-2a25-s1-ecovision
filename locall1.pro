QT += core gui widgets sql charts

CONFIG += c++17

TEMPLATE = app
TARGET = locall

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    local.cpp \
    reclamation.cpp

HEADERS += \
    mainwindow.h \
    connection.h \
    local.h \
    reclamation.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
