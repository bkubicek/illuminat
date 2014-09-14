#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T10:20:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = illuminat
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -O3

SOURCES += main.cpp\
        mainwindow.cpp \
        triface.cpp \
		stlfile.cpp \
		layer.cpp

HEADERS  += mainwindow.h \
		triface.h \
		stlfile.h \
		layer.h