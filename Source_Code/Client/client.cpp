#include "client.h" // Inclusie van de header file voor de Client klasse. Deze file bevat de definitie van de Client klasse.

#include <iostream> // Standaard C++ bibliotheek voor input en output operaties.

#include <string> // Standaard C++ bibliotheek voor string manipulatie.

Client::Client() // Constructor voor de Client klasse. Deze functie wordt aangeroepen wanneer een nieuw object van de Client klasse wordt gecreëerd.
{
    ID = std::to_string((rand() % 100000) + 1); // Genereer een unieke ID voor de client door een willekeurig getal te nemen en dit om te zetten naar een string.

    // Definieer en initialiseer de topics voor de calculator en random number services met de gegenereerde ID.
    subscribeTopicCalculator.append(ID + ">");
    pushTopicCalculator.append(ID + ">");
    subscribeTopicRandomNumber.append(ID + ">");
    pushTopicRandomNumber.append(ID + ">");

    // Verbind met de Benternet push en subscribe sockets.
    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscribeSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");

    // Stel de ZeroMQ socket opties in om te abonneren op de gedefinieerde topics.
    subscribeSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopicCalculator.c_str(), subscribeTopicCalculator.length());
    subscribeSocket->setsockopt(ZMQ_SUBSCRIBE, subscribeTopicRandomNumber.c_str(), subscribeTopicRandomNumber.length());

    try // Start een try-catch blok om fouten te vangen.
    {
        zmq::message_t *msg = new zmq::message_t(); // Creëer een nieuw ZeroMQ bericht.

        while (pushSocket->connected()) // Zolang de push socket verbonden is, blijf de volgende instructies uitvoeren.
        {
            int choice; // Variabele om de keuze van de gebruiker op te slaan.

            // Vraag de gebruiker om een service te selecteren.
            std::cout << "Je kan een formule sturen of random getal genereren." << std::endl;
            std::cout << "Welke service wil je gebruiken?" << std::endl;
            std::cout << "Geef 1 in voor de calculator en 2 voor de nummer generator: ";
            std::cin >> choice;

            // Valideer de keuze van de gebruiker.
            if (choice != 1 && choice != 2)
            {
                std::cout << "Geen geldige keuze!" << std::endl;
                std::cout << "Geef 1 in voor de calculator en 2 voor de nummer generator: ";
                std::cin >> choice;
            }

            if (choice == 1) // Als de gebruiker voor de calculator service kiest...
            {
                std::string Formula; // Variabele om de invoerformule op te slaan.

                // Vraag de gebruiker om een formule in te voeren.
                std::cout << std::endl;
                std::cout << "Je kan '+' '-' '*' '/' 'sin' 'cos' en 'tan' gebruiken in je formule;" << std::endl;
                std::cout << "Geef de gewenste formule in: ";
                std::cin >> Formula;
                std::cout << std::endl;

                std::string fullSend = pushTopicCalculator + Formula; // Creëer de volledige bericht string om te sturen.

                system("pause"); // Pauzeer het systeem. Wacht op een toetsaanslag van de gebruiker.

                pushSocket->send(fullSend.c_str(), fullSend.length()); // Stuur het bericht naar de server.

                subscribeSocket->recv(msg); // Ontvang het antwoord van de server.

                // Parse het ontvangen bericht om het antwoord te extraheren.
                std::string message((char *)msg->data(), msg->size());
                std::string output = message.substr(subscribeTopicCalculator.length());

                // Print het antwoord.
                std::cout << "Antwoord: " << output << std::endl;
                std::cout << std::endl;
                std::cout << "------------------------------------------------------------------------" << std::endl;
                std::cout << std::endl;
            }

            else if (choice == 2) // Als de gebruiker voor de random number generator service kiest...
            {
                std::string fullSend = pushTopicRandomNumber; // Creëer de volledige bericht string om te sturen.

                std::cout << std::endl;
                system("pause"); // Pauzeer het systeem. Wacht op een toetsaanslag van de gebruiker.

                pushSocket->send(fullSend.c_str(), fullSend.length()); // Stuur het bericht naar de server.

                subscribeSocket->recv(msg); // Ontvang het antwoord van de server.

                // Parse het ontvangen bericht om het antwoord te extraheren.
                std::string message((char *)msg->data(), msg->size());
                std::string output = message.substr(subscribeTopicRandomNumber.length());

                // Print het antwoord.
                std::cout << "Random getal: " << output << std::endl;
                std::cout << std::endl;
                std::cout << "------------------------------------------------------------------------" << std::endl;
                std::cout << std::endl;
            }
        }
    }
    catch (zmq::error_t &ex) // Als er een uitzondering optreedt, vang deze dan.
    {
        std::cerr << "Uitzondering gevonden: " << ex.what(); // Print de uitzondering.
    }
}
