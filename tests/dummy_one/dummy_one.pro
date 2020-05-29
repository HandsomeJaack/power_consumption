TARGET = dummy_one
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT += core

target.path += /usr/bin
INSTALLS += target

SOURCES += \
	dummy_one.cpp
