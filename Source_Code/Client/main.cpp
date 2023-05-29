// stuurt

#include <iostream>
#include <zmq.hpp>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n) Sleep(n)
#endif

#include "client.h"

int main(void)
{
    // Initialiseren van de random seed
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Client client;
    return 0;
}
