#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T13:11:05
#
#-------------------------------------------------
QMAKE_MAC_SDK = macosx10.11
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TotalPredictionClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileselect.cpp \
    testwindow.cpp \
    vieweditchoice.cpp \
    vieweditusers.cpp \
    viewedittests.cpp

HEADERS  += mainwindow.h \
    fileselect.h \
    testwindow.h \
    vieweditchoice.h \
    vieweditusers.h \
    viewedittests.h

FORMS    += mainwindow.ui \
    fileselect.ui \
    testwindow.ui \
    vieweditchoice.ui \
    vieweditusers.ui \
    viewedittests.ui

RESOURCES += \
    resources.qrc
