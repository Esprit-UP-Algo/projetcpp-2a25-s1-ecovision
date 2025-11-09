#-------------------------------------------------
# Projet de Gestion des Employés avec Statistiques
# Qt Project File (.pro)
#-------------------------------------------------

QT       += core gui widgets sql charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Supprime les warnings pour les API dépréciées
# Décommentez pour activer la vérification stricte
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Fichiers sources (.cpp)
SOURCES += \
    connectdb.cpp \
    connection.cpp \
    employee.cpp \
    main.cpp \
    mainwindow.cpp

# Fichiers d'en-tête (.h)
HEADERS += \
    connectdb.h \
    connection.h \
    employee.h \
    mainwindow.h

# Fichiers de formulaires Qt Designer (.ui)
FORMS += \
    connection.ui \
    mainwindow.ui

# Configuration des ressources (si vous avez des images/icônes)
# Décommentez et créez le fichier resources.qrc si nécessaire
#RESOURCES += \
#    resources.qrc

# Règles de déploiement par défaut
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Configuration spécifique pour Windows
win32 {
    CONFIG += console
}

# Configuration pour activer les optimisations
release {
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -O3
}

# Afficher plus de warnings du compilateur
QMAKE_CXXFLAGS += -Wall
