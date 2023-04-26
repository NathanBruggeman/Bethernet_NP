TEMPLATE = app
CONFIG += console c++11
CONFIG += core
CONFIG += core5compat
CONFIG -= app_bundle

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/../../../lib -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../../../include

SOURCES += main.cpp \
    service.cpp

HEADERS += \
    service.h
