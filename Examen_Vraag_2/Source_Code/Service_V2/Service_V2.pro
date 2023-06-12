TEMPLATE = app
CONFIG += console c++11
CONFIG += core
CONFIG += core5compat
CONFIG -= app_bundle

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/../../../lib -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../../../include

SOURCES += main_V2.cpp \
    service_V2.cpp

HEADERS += \
    service_V2.h

QT += network
QT += widgets
