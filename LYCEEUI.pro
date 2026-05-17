QT += core gui sql widgets

CONFIG += c++17

# IMPORTANT : On dit au compilateur de chercher les headers dans ces dossiers
INCLUDEPATH += database managers models ui

SOURCES += \
    main.cpp \
    managers/adminManager.cpp \
    managers/passwordutils.cpp \
    managers/sessionmanager.cpp \
    models/cours.cpp \
    models/enseignant.cpp \
    models/etudiant.cpp \
    models/personne.cpp \
    models/session.cpp \
    models/ville.cpp \
    ui/formationpage.cpp \
    ui/formationwidget.cpp \
    ui/filedroplineedit.cpp \
    ui/loginpage.cpp \
    ui/passwordchangedialog.cpp \
    ui/adminpage.cpp \
    ui/enseignantpage.cpp \
    ui/etudiantspage.cpp \
    ui/mainwindow.cpp \
    ui/welcomescreen.cpp \
    database/dbConnexion.cpp \
    managers/scolariteManager.cpp \
    models/formation.cpp

HEADERS += \
    managers/adminManager.h \
    managers/passwordutils.h \
    managers/sessionmanager.h \
    models/cours.h \
    models/enseignant.h \
    models/etudiant.h \
    models/personne.h \
    models/session.h \
    models/ville.h \
    ui/formationpage.h \
    ui/formationwidget.h \
    ui/filedroplineedit.h \
    ui/loginpage.h \
    ui/passwordchangedialog.h \
    ui/adminpage.h \
    ui/enseignantpage.h \
    ui/etudiantspage.h \
    ui/mainwindow.h \
    ui/welcomescreen.h \
    database/dbConnexion.h \
    managers/scolariteManager.h \
    models/formation.h

FORMS += \


RESOURCES += \
    ressources.qrc
