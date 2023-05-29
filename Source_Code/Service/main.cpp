// ontvanger

#include <iostream>
#include <string>
#include <zmq.hpp>

#include <string.h>

#include "service.h"

#include <cstdlib> // For rand() and srand() functions
#include <ctime>   // For time() function

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator with the current time

    service service;
    service.getQuestion();

    return a.exec();
}
