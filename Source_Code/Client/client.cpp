#include "client.h"
#include <iostream>
#include <string>

Client::Client()
{
    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscribeSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscribeSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopicCalculator.c_str(), subscribeTopicCalculator.length());
    subscribeSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopicRandomNumber.c_str(), subscribeTopicRandomNumber.length());

    try
    {
        zmq::message_t *msg = new zmq::message_t();
        while (pushSocket->connected())
        {
            int choice;
            std::cout << "Je kan een formule sturen of random getal genereren." << std::endl;
            std::cout << "Welke service wil je gebruiken?" << std::endl;
            std::cout << "Geef 1 in voor de calculator en 2 voor de nummer generator: ";
            std::cin >> choice;
            if (choice != 1 && choice != 2)
            {
                std::cout << "Geen geldige keuze!" << std::endl;
                std::cout << "Geef 1 in voor de calculator en 2 voor de nummer generator: ";
                std::cin >> choice;
            }

            if (choice == 1)
            {

                std::string Formula;
                std::cout << "je kan '+' '-' '*' '/' 'sin' 'cos' en 'tan' gebruiken in je formule;" << std::endl;
                std::cout << "Geef de gewenste formule in: ";
                std::cin >> Formula;
                std::cout << std::endl;

                std::string fullSend = pushTopicCalculator + Formula;
                system("pause");
                pushSocket->send(fullSend.c_str(), fullSend.length());
                subscribeSocket->recv(msg);
                std::string message((char *)msg->data(), msg->size());
                std::string output = message.substr(subscribeTopicCalculator.length()); // Parsing
                std::cout << "Antwoord:" << output << std::endl;
                std::cout << std::endl;
            }

            else if (choice == 2)
            {

                std::string fullSend = pushTopicRandomNumber;
                system("pause");
                pushSocket->send(fullSend.c_str(), fullSend.length());
                subscribeSocket->recv(msg);

                std::cout << std::endl;
                std::string message((char *)msg->data(), msg->size());
                std::string output = message.substr(subscribeTopicRandomNumber.length()); // Parsing
                std::cout << "Random getal: " << output << std::endl;
                std::cout << std::endl;
            }
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Uitzondering gevonden: " << ex.what();
    }
}
