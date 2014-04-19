#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T13:05:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Path_Planning_AG-E
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mouselisteningscene.cpp \
    pathfinder.cpp

HEADERS  += mainwindow.h \
    mouselisteningscene.h \
    pathfinder.h

FORMS    += mainwindow.ui
