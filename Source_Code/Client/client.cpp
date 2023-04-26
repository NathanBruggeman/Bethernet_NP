#include "client.h"
#include <iostream>
#include <string>

Client::Client()
{
    PushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");
    SubscribeSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");
    SubscribeSocket->setsockopt(ZMQ_SUBSCRIBE, SubscribeTopic.c_str(), SubscribeTopic.length());

    try
    {
        zmq::message_t *msg = new zmq::message_t();
        while (PushSocket->connected())
        {
            std::string Formule;
            std::cout << "Geef de gewenste formule in: ";
            std::cin >> Formule;

            std::string Full_Send = PushTopic + Formule;
            system("pause");
            PushSocket->send(Full_Send.c_str(), Full_Send.length());
            std::cout << "Pushed : [ping]" << std::endl;
            SubscribeSocket->recv(msg);
            std::string message((char *)msg->data(), msg->size());
            std::cout << message << std::endl;
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Caught an exception 1 : " << ex.what();
    }
}
