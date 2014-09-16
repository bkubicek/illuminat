#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T10:20:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = illuminat
TEMPLATE = app

QMAKE_CXXFLAGS +=  -O3
#profile:
#QMAKE_CXXFLAGS +=  -g -pg
#QMAKE_LFLAGS +=  -g -pg

SOURCES += main.cpp\
        mainwindow.cpp \
        triface.cpp \
		stlfile.cpp \
		layer.cpp \
    motion.cpp \
    illuminator.cpp

HEADERS  += mainwindow.h \
		triface.h \
		stlfile.h \
		layer.h \
    motion.h \
    illuminator.h

OTHER_FILES += \
    settings.ini
