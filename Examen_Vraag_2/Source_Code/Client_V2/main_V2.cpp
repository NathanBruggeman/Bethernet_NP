#include <iostream> // Standaard C++ bibliotheek voor input en output operaties.

#include <zmq.hpp> // Inclusie van de ZeroMQ bibliotheek. Deze bibliotheek wordt gebruikt voor messaging tussen de client en server.

// Platform-specifieke includes en definities.
#ifndef _WIN32            // Als het besturingssysteem geen Windows is...
#include <unistd.h>       // Inclusie van de unistd.h bibliotheek voor Unix-achtige besturingssystemen.
#else                     // Als het besturingssysteem Windows is...
#include <windows.h>      // Inclusie van de windows.h bibliotheek voor Windows.
#define sleep(n) Sleep(n) // Definieer de sleep functie op basis van de Windows Sleep functie.
#endif

#include "client_V2.h" // Inclusie van de header file voor de Client klasse.

int main(void) // Hoofdfunctie van het programma.
{
    // Initialiseren van de random seed op basis van de huidige tijd.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Client client; // Creëer een instantie van de Client klasse.

    return 0; // Beëindig de hoofdfunctie en geef 0 terug aan het besturingssysteem.
}
