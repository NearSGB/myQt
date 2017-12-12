#-------------------------------------------------
#
# Project created by QtCreator 2017-12-07T14:43:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Screen_4_USB
TEMPLATE = app
INCLUDEPATH+=d:/opencv/build/include/opencv \
            d:/opencv/build/include/opencv2 \
            d:/opencv/build/include
LIBS+=D:\opencv\build\x64\vc14\lib\opencv_world331.lib

#LIBS+=D:\opencv\build\x64\vc14\lib\opencv_world331d.lib
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
