TARGET = dummy_two
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT += core

target.path += /usr/bin
INSTALLS += target

SOURCES += \
    dummy_two.cpp \

HEADERS += \
    worker.h
