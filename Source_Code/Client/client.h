#ifndef CLIENT_H // Zorg ervoor dat deze header file slechts eenmaal wordt geïncludeerd.
#define CLIENT_H // Definieer de CLIENT_H macro.

#include <iostream> // Standaard C++ bibliotheek voor input en output operaties.

#include <zmq.hpp> // Inclusie van de ZeroMQ bibliotheek. Deze bibliotheek wordt gebruikt voor messaging tussen de client en server.

// Platform-specifieke includes en definities.
#ifndef _WIN32            // Als het besturingssysteem geen Windows is...
#include <unistd.h>       // Inclusie van de unistd.h bibliotheek voor Unix-achtige besturingssystemen.
#else                     // Als het besturingssysteem Windows is...
#include <windows.h>      // Inclusie van de windows.h bibliotheek voor Windows.
#define sleep(n) Sleep(n) // Definieer de sleep functie op basis van de Windows Sleep functie.
#endif

class Client // Definitie van de Client klasse.
{
public:
    Client(); // Public constructor voor de Client klasse.

private:
    // Definitie en initialisatie van de topics voor de calculator en random number services.
    std::string subscribeTopicCalculator = "Nathan>service!>calculator>";
    std::string pushTopicCalculator = "Nathan>service?>calculator>";
    std::string subscribeTopicRandomNumber = "Nathan>service!>randomnumber>";
    std::string pushTopicRandomNumber = "Nathan>service?>randomnumber>";

    // Initialisatie van de ZeroMQ context en sockets.
    zmq::context_t *context = new zmq::context_t(1);
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subscribeSocket = new zmq::socket_t(*context, ZMQ_SUB);

    std::string ID; // Definitie van de ID variabele voor de client.
};

#endif // CLIENT_H
