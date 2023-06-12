#ifndef CLIENT_V2_H
#define CLIENT_V2_H

#include <iostream>
#include <string>
#include <zmq.hpp>

#include <string.h>

class Client
{
public:
    Client();

private:
    std::string subscribeTopicCalculator = "Nathan>service!>calculator>";
    std::string pushTopicCalculator = "Nathan>service?>calculator>";
    std::string subscribeTopicRandomNumber = "Nathan>service!>randomnumber>";
    std::string pushTopicRandomNumber = "Nathan>service?>randomnumber>";

    std::string subscribeTopicStore = "Nathan>service!>store>";
    std::string pushTopicStore = "Nathan>service?>store>";

    zmq::context_t *context = new zmq::context_t(1);
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subscribeSocket = new zmq::socket_t(*context, ZMQ_SUB);

    std::string ID;
};

#endif // CLIENT_V2_H
