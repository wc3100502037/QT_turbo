#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T10:50:44
#
#-------------------------------------------------

QT       += core gui
QT       +=sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Turbo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    smoothcurvecreator.cpp \
    paintcurve.cpp

HEADERS  += mainwindow.h \
    smoothcurvecreator.h \
    paintcurve.h

FORMS    += mainwindow.ui
