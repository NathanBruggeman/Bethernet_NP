#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <zmq.hpp>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

class Client
{
public:
    Client();

private:
    std::string SubscribeTopic = "Nathan>service!>";
    std::string PushTopic = "Nathan>service?>";
    zmq::context_t *context = new zmq::context_t(1);
    zmq::socket_t *PushSocket = new zmq::socket_t ( *context, ZMQ_PUSH );
    zmq::socket_t *SubscribeSocket = new zmq::socket_t ( *context, ZMQ_SUB );
};

#endif // CLIENT_H
