QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bargrapdiaform.cpp \
    graficadiaform.cpp \
    graficasresultadosform.cpp \
    main.cpp \
    mainwindow.cpp \
    libs/sqlite3.c

HEADERS += \
    bargrapdiaform.h \
    graficadiaform.h \
    graficasresultadosform.h \
    mainwindow.h \
    libs/sqlite3.h

FORMS += \
    bargrapdiaform.ui \
    graficadiaform.ui \
    graficasresultadosform.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = vosegoico.ico
