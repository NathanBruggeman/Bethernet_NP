// ontvanger

#include <iostream>
#include <string>
#include <zmq.hpp>

#include <string.h>

#include "service.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    service service;
    service.getQuestion();

    return a.exec();
}
