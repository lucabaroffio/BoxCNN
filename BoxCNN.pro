#-------------------------------------------------
#
# Project created by QtCreator 2014-11-03T15:15:39
#
#-------------------------------------------------

#QT       += core

#QT       -= gui

INCLUDEPATH += /usr/local/include \
               /opt/local/include \

LIBS += -L/opt/local/lib \
     -lopencv_core \
     -lopencv_imgproc \
     -lopencv_features2d \
     -lopencv_calib3d \
     -lopencv_nonfree \
     -lopencv_highgui \
     -lboost_system-mt \
     -lboost_thread-mt \

TARGET = BoxCNN
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    operationparameters.cpp \
    boxfilterparameters.cpp \
    summedvolumetable.cpp \
    cnn_operation.cpp \
    boxfilter.cpp \
    boxfilterbank.cpp \
    maxpooling.cpp

HEADERS += \
    operationparameters.h \
    boxfilterparameters.h \
    summedvolumetable.h \
    cnn_operation.h \
    boxfilter.h \
    boxfilterbank.h \
    maxpooling.h
