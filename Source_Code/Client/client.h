#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <zmq.hpp>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n) Sleep(n)
#endif

class Client
{
public:
    Client();

private:
    std::string subscribeTopicCalculator = "Nathan>service!>calculator>";
    std::string pushTopicCalculator = "Nathan>service?>calculator>";
    std::string subscribeTopicRandomNumber = "Nathan>service!>randomnumber>";
    std::string pushTopicRandomNumber = "Nathan>service?>randomnumber>";
    zmq::context_t *context = new zmq::context_t(1);
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subscribeSocket = new zmq::socket_t(*context, ZMQ_SUB);
};

#endif // CLIENT_H
