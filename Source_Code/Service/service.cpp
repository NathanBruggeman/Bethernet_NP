#include "service.h"
#include <iostream>
#include <string>

service::service()
{
    subSocket->setsockopt(ZMQ_SUBSCRIBE, SubscribeTopic.c_str(), SubscribeTopic.length());
       subSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");
       pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");

    /* Push->connect( "tcp://benternet.pxl-ea-ict.be:24041" );
    socket->connect( "tcp://benternet.pxl-ea-ict.be:24042" );
    socket->setsockopt( ZMQ_SUBSCRIBE, SubscribeTopic.c_str(), SubscribeTopic.length());
*/
    try
    {
        while( subSocket->connected() )
        {
            subSocket->recv( zmqBuffer );
            std::string message ( (char*) zmqBuffer->data(), zmqBuffer->size() );

            //Check if the string contains "1+1"
              (message.find("1+1") != std::string::npos);
                   std::string buffer = PushTopic;
                    buffer += 2;
                   pushSocket->send(buffer.c_str(), buffer.length());

            std::cout << message << std::endl;
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
}
