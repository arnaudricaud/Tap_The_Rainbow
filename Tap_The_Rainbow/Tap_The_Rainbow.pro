#-------------------------------------------------
#
# Project created by QtCreator 2017-04-04T14:30:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tap_The_Rainbow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    traitement_images.cpp

HEADERS  += mainwindow.h \
    traitement_images.h

FORMS    += mainwindow.ui

INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core2413 \
    -lopencv_highgui2413 \
    -lopencv_imgproc2413 \
    -lopencv_features2d2413 \
    -lopencv_calib3d2413


