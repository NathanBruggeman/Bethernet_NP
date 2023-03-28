// stuurt

#include <iostream>
#include <zmq.hpp>
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

#include "client.h"

int main( void )
{
    Client client;
    return 0;
}
