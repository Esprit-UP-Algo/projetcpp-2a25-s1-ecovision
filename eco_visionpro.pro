QT += widgets sql printsupport charts core gui
CONFIG += c++17

INCLUDEPATH += $$PWD/libs/qrcodegen

SOURCES += \
    connection.cpp \
    employee.cpp \
    main.cpp \
    mainwindow.cpp \
    resource.cpp \
    statwindow.cpp \
    libs/qrcodegen/QrCode.cpp

HEADERS += \
    connection.h \
    employee.h \
    mainwindow.h \
    resource.h \
    statwindow.h \
    libs/qrcodegen/QrCode.hpp

FORMS += \
    mainwindow.ui

