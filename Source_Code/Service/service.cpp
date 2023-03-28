#include "service.h"
#include <iostream>
#include <string>

service::service()
{
    Push->connect( "tcp://benternet.pxl-ea-ict.be:24041" );
    socket->connect( "tcp://benternet.pxl-ea-ict.be:24042" );
    socket->setsockopt( ZMQ_SUBSCRIBE, SubscribeTopic.c_str(), SubscribeTopic.length());

    try
    {
        zmq::message_t * msg = new zmq::message_t();
        while( socket->connected() )
        {
            socket->recv( msg );
            std::string message ( (char*) msg->data(), msg->size() );

            // Check if the string contains "1+1"
               if (message.find("1+1") != std::string::npos) {
                   socket->send("2", 1);
               } else {
                   std::cout << "The string does not contain \"1+1\"" << std::endl;
               }

            std::cout << message << std::endl;
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
}
