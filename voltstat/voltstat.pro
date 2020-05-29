TARGET = voltstat
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT += core

target.path += /usr/bin
INSTALLS += target

SOURCES += \
    voltstat.cpp \
