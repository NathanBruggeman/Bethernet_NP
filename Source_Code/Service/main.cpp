// ontvanger

#include <iostream>
#include <string>
#include <zmq.hpp>

#include <string.h>

int main( void )
{
    try
    {
        zmq::context_t context(1);

        //Incoming messages come in here
        zmq::socket_t subscriber( context, ZMQ_SUB );
//		subscriber.connect( "tcp://192.168.1.8:24042" );
//		subscriber.connect( "tcp://localhost:24042" );
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "Nathan>service?>", strlen("Nathan>service?>" ));

        zmq::message_t * msg = new zmq::message_t();
        while( subscriber.connected() )
        {
            subscriber.recv( msg );
            std::string message ( (char*) msg->data(), msg->size() );

            int result = message.compare("Nathan>service?>1+1");

                if (result == 0)
                    std::cout << "\nBoth the input strings are equal." << std::endl;
                else if (result < 0)
                    std::cout << "\nString 1 is smaller as compared to String 2." << std::endl;
                else
                    std::cout << "\nString 1 is greater as compared to String 2." << std::endl;

            std::cout << message << std::endl;
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}
