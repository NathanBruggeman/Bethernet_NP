#include "service.h" //
#include <iostream>
#include <string>

#include <fstream> // For file handling

service::service()
{
    subSocket->setsockopt(ZMQ_SUBSCRIBE, SubscribeTopicCalculator.c_str(), SubscribeTopicCalculator.length());
    subSocket->setsockopt(ZMQ_SUBSCRIBE, SubscribeTopicRandomNumber.c_str(), SubscribeTopicRandomNumber.length());
    subSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");
    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");
    networkManager = new QNetworkAccessManager();

    /* Push->connect( "tcp://benternet.pxl-ea-ict.be:24041" );
    socket->connect( "tcp://benternet.pxl-ea-ict.be:24042" );
    socket->setsockopt( ZMQ_SUBSCRIBE, SubscribeTopic.c_str(), SubscribeTopic.length());S
*/
}

void service::getQuestion()
{
    try
    {
        if (subSocket->connected())
        {

            subSocket->recv(zmqBuffer);
            std::string message((char *)zmqBuffer->data(), zmqBuffer->size());

            // kijken welke topic
            QString messageQstring = message.c_str();
            const QStringList parsedBuffer = messageQstring.split(">");
            const QString topic = parsedBuffer.value(2);
            const QString userId = parsedBuffer.value(3);

            if (topic == "calculator")
            {
                // logic
                std::string Buffer = GetPushTopicCalculator() + userId.toStdString();
                std::string output = message.substr(Buffer.length() + 1); // message.find( ">", SubscribeTopic.length()));

                makeHttpRequest(output.c_str(), userId);
            }

            else if (topic == "randomnumber")
            {
                generateRandomNum(userId);
            }
        }
    }

    catch (zmq::error_t &ex)
    {
        std::cerr << "Uitzondering gevonden : " << ex.what();
    }
}

void service::makeHttpRequest(QString Formula, QString userId)
{
    qInfo() << "Formule =" << Formula;

    QString Buffer = "https://api.mathjs.org/v4/?expr=";
    Buffer += QUrl::toPercentEncoding(Formula);
    QUrl url(Buffer);
    qInfo() << url;
    QNetworkRequest request(url);

    QNetworkReply *reply = networkManager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=]()
                     {
        QByteArray responseData = reply->readAll();

        std::cout << std::endl;
        qInfo() << "Answer =" << responseData;
        std::cout << std::endl;
        // Do something with responseData
        reply->deleteLater();
        //networkManager->deleteLater();

        // terugsturen naar Client
        QString messageToClient = PushTopicCalculator.c_str() + userId + ">" + responseData;
        std::cout << userId.toStdString() << std::endl;
        std::cout << messageToClient.toStdString() << std::endl; // edit ID
        pushSocket->send(messageToClient.toStdString().c_str(), messageToClient.length());

        std::ofstream outputFile("Formules_en_Antwoorden.csv", std::ios::app);

            // kijk of file geopend is
            if (!outputFile) {
                std::cerr << "Error bij het openen van de file." << std::endl;
            }

            // schrijf de formule in de file
            outputFile << Formula.toStdString() << std::endl;
            outputFile << responseData.toStdString() << std::endl;

            std::cout << "Random Number gesaved in random_numbers.csv" << std::endl;

        getQuestion(); });
}

void service::generateRandomNum(QString userId)
{
    try
    {
        if (subSocket->connected())
        {

            // maak random number
            int randomNumber = std::rand() % 101; // Generate a random number between 0 and 100

            std::ofstream outputFile("random_numbers.csv", std::ios::app);

                // kijk of de file geopend is
                if (!outputFile) {
                    std::cerr << "Error bij het openen van de file." << std::endl;
                }

                // schrijf het random getal in de file
                outputFile << randomNumber << std::endl;

            // terugsturen naar Client
            QString messageToClient = PushTopicRandomNumber.c_str() + userId + ">"; // + ID
            messageToClient.append(std::to_string(randomNumber).c_str());
            std::cout << messageToClient.toStdString() << std::endl;
            std::cout << "Gegenereerd random getal = " << randomNumber << std::endl;
            std::cout << std::endl;
            pushSocket->send(messageToClient.toStdString().c_str(), messageToClient.length());
            std::cout << "Random Number gesaved in random_numbers.csv" << std::endl;
            getQuestion();
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Uitzondering gevonden : " << ex.what();
    }
}
