#ifndef SERVICE_H // Zorg ervoor dat deze header file slechts eenmaal wordt geïncludeerd.
#define SERVICE_H // Definieer de SERVICE_H macro.

#include <iostream> // Standaard C++ bibliotheek voor input en output operaties.
#include <zmq.hpp>  // Inclusie van de ZeroMQ bibliotheek. Deze bibliotheek wordt gebruikt voor messaging tussen de client en server.

// Platform-specifieke includes en definities.
#ifndef _WIN32            // Als het besturingssysteem geen Windows is...
#include <unistd.h>       // Inclusie van de unistd.h bibliotheek voor Unix-achtige besturingssystemen.
#else                     // Als het besturingssysteem Windows is...
#include <windows.h>      // Inclusie van de windows.h bibliotheek voor Windows.
#define sleep(n) Sleep(n) // Definieer de sleep functie op basis van de Windows Sleep functie.
#endif

// Inclusie van verschillende Qt bibliotheken voor netwerkfuncties en debugging.
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <QDebug>

class service // Definitie van de service klasse.
{
public:
    service(); // Public constructor voor de service klasse.

    // Getter functies voor de verschillende topics.
    std::string GetPushTopicCalculator(void)
    {
        return PushTopicCalculator;
    }
    std::string GetSubTopicCalculator(void)
    {
        return SubscribeTopicCalculator;
    }
    std::string GetPushTopicRandomNumber(void)
    {
        return PushTopicRandomNumber;
    }
    std::string GetSubTopicRandomnumber(void)
    {
        return SubscribeTopicRandomNumber;
    }

    // Functies voor het maken van HTTP-verzoeken en het genereren van willekeurige getallen.
    void makeHttpRequest(QString Formule, QString userId);
    void getQuestion();
    void generateRandomNum(QString userId);
    std::string replaceAnsWithValue(const std::string &formula);
    // Functie om de waarde van "ans" in te stellen
       void setAns(double value);

private:
    // Definitie en initialisatie van de topics voor de calculator en random number services.
    std::string SubscribeTopicCalculator = "Nathan>service?>calculator>";
    std::string PushTopicCalculator = "Nathan>service!>calculator>";
    std::string SubscribeTopicRandomNumber = "Nathan>service?>randomnumber>";
    std::string PushTopicRandomNumber = "Nathan>service!>randomnumber>";

    // Initialisatie van de ZeroMQ context en sockets.
    zmq::context_t *context = new zmq::context_t(1);
    zmq::message_t *zmqBuffer = new zmq::message_t();
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subSocket = new zmq::socket_t(*context, ZMQ_SUB);

    // Initialisatie van de QNetworkAccessManager voor het maken van netwerkverzoeken.
    QNetworkAccessManager *networkManager;

     double ans; // Variabele om het resultaat bij te houden
};

#endif // SERVICE_H
