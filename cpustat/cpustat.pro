TARGET = cpustat
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT += core

target.path += /usr/bin
INSTALLS += target

SOURCES += \
    cpustat.cpp