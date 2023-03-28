#ifndef SERVICE_H
#define SERVICE_H

#include <iostream>
#include <zmq.hpp>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

class service
{
public:
    service();

private:
    std::string SubscribeTopic = "Nathan>service?>";
    std::string PushTopic = "Nathan>service!>";
    zmq::context_t *context = new zmq::context_t(1);
    zmq::socket_t *socket = new zmq::socket_t ( *context, ZMQ_SUB );
    zmq::socket_t *Push = new zmq::socket_t ( *context, ZMQ_PUSH );
};

#endif // SERVICE_H
