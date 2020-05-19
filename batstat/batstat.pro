TARGET = batstat
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT += core

target.path += /usr/bin
INSTALLS += target

SOURCES += \
    batstat.cpp

#module.commands = make -C ../freqvolmod
#QMAKE_EXTRA_TARGETS += module
#PRE_TARGETDEPS += module
