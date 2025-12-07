


QT       += core gui sql network printsupport widgets charts \
    quick


QT += charts
QT       += core gui charts
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    backup.cpp \
    connection.cpp \
    import.cpp \
    main.cpp \
    mainwindow.cpp \
    parking.cpp \
    qrcodegen.cpp


HEADERS += \
    arduino.h \
    backup.h \
    connection.h \
    import.h \
    mainwindow.h \
    parking.h \
    qrcodegen.hpp


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
