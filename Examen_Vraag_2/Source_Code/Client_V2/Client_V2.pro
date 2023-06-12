TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/../../../lib -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../../../include

SOURCES += main_V2.cpp \
    client_V2.cpp

HEADERS += \
    client_V2.h
