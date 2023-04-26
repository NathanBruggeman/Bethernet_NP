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
        while (subSocket->connected())
        {
            subSocket->recv(zmqBuffer);
            std::string message((char *)zmqBuffer->data(), zmqBuffer->size());

            // logic
            std::string Buffer = GetPushTopic();
            std::string output = message.substr(message.find(">") + 10);
            std::cout << output << std::endl;

            // extract numbers and symbol from input string
            int num1 = stoi(output.substr(0, output.find("+")));
            int num2 = stoi(output.substr(output.find("+") + 1, output.length()));
            char symbol = '+';

            // perform arithmetic operation based on symbol
            int result = 0;
            switch (symbol) {
                case '+':
                    result = num1 + num2;
                    break;
                case '-':
                    result = num1 - num2;
                    break;
                case '*':
                    result = num1 * num2;
                    break;
                case '/':
                    result = num1 / num2;
                    break;
                default:
                    std::cout << "Invalid symbol." << std::endl;
            }

            Buffer.append(std::to_string(result));
            pushSocket->send(Buffer.c_str(), Buffer.length());

            std::cout << message << std::endl;
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
}
