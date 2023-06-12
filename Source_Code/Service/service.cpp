#include "service.h" // Including the header file for the service class

#include <iostream> // For standard I/O operations (like cout and cin)
#include <string> // For using the string class

#include <fstream> // For file handling



#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>



service::service() // Constructor for the service class
{
    // Subscribe to topics on the socket
    subSocket->setsockopt(ZMQ_SUBSCRIBE, SubscribeTopicCalculator.c_str(), SubscribeTopicCalculator.length());
    subSocket->setsockopt(ZMQ_SUBSCRIBE, SubscribeTopicRandomNumber.c_str(), SubscribeTopicRandomNumber.length());

    // Connect the socket to the broker
    subSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");
    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");

    // Instantiate the QNetworkAccessManager object
    networkManager = new QNetworkAccessManager();
}

void service::getQuestion() // Method to get the question
{
    try
    {
        // If the socket is connected
        if (subSocket->connected())
        {
            // Receive the message from the socket
            subSocket->recv(zmqBuffer);

            // Convert the message to a string
            std::string message((char *)zmqBuffer->data(), zmqBuffer->size());

            // Parse the message
            QString messageQstring = message.c_str();
            const QStringList parsedBuffer = messageQstring.split(">");
            const QString topic = parsedBuffer.value(2);
            const QString userId = parsedBuffer.value(3);

            // If the topic is calculator
            if (topic == "calculator")
            {
                std::string Buffer = GetPushTopicCalculator() + userId.toStdString();
                       std::string output = message.substr(Buffer.length() + 1);

                       // Replace "ans" with the stored value
                       output = replaceAnsWithValue(output);

                       // Make the HTTP request
                       makeHttpRequest(output.c_str(), userId);
            }
            // If the topic is random number
            else if (topic == "randomnumber")
            {
                // Generate the random number
                generateRandomNum(userId);
            }
        }
    }
    // Catch exceptions
    catch (zmq::error_t &ex)
    {
        std::cerr << "Uitzondering gevonden : " << ex.what();
    }
}

void service::makeHttpRequest(QString Formula, QString userId) // Method to make an HTTP request
{
    qInfo() << "Formule =" << Formula;

    // Format the URL
    QString Buffer = "https://api.mathjs.org/v4/?expr=";
    Buffer += QUrl::toPercentEncoding(Formula);
    QUrl url(Buffer);
    qInfo() << url;
    QNetworkRequest request(url);

    // Make the GET request
        QNetworkReply *reply = networkManager->get(request);

        QObject::connect(reply, &QNetworkReply::finished, [=]() {
            QByteArray responseData = reply->readAll();
            qDebug() << responseData;

            // Check if the response contains an error
            if (responseData.startsWith("Error"))
            {
                std::cout << "API Error: " <<  std::endl;
            }
            else
            {
                std::cout << "test1" << std::endl;
                qDebug() << responseData;
                ans = responseData.toDouble();
                std::cout << ans << std::endl;
            }

        std::cout << std::endl;
        qInfo() << "Answer =" << responseData;
        std::cout << std::endl;

        // Delete the reply object
        reply->deleteLater();

        // Format the message to the client // terugsturen naar Client
        QString messageToClient = PushTopicCalculator.c_str() + userId + ">" + responseData;

        std::cout << userId.toStdString() << std::endl;
        std::cout << messageToClient.toStdString() << std::endl; // edit ID

        // Send the message to the client
        pushSocket->send(messageToClient.toStdString().c_str(), messageToClient.length());

        // Open the output file
        std::ofstream outputFile("Formules_en_Antwoorden.csv", std::ios::app);

        // Check if the file is open
        if (!outputFile) {
            std::cerr << "Error bij het openen van de file." << std::endl;
        }

        // Write to the file
        outputFile << Formula.toStdString() << std::endl;
        outputFile << responseData.toStdString() << std::endl;

        std::cout << "Formule en antwoord gesaved in Formules_en_Antwoorden.csv" << std::endl;
        std::cout << std::endl;
        std::cout << "------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;

        getQuestion();
    });
}

void service::generateRandomNum(QString userId) // Method to generate a random number
{
    try
    {
        if (subSocket->connected())
        {
            // Generate the random number
            int randomNumber = std::rand() % 101;

            // Open the output file
            std::ofstream outputFile("random_numbers.csv", std::ios::app);

            // Check if the file is open
            if (!outputFile) {
                std::cerr << "Error bij het openen van de file." << std::endl;
            }

            // Write to the file
            outputFile << randomNumber << std::endl;

            // Format the message to the client
            QString messageToClient = PushTopicRandomNumber.c_str() + userId + ">";
            messageToClient.append(std::to_string(randomNumber).c_str());

            std::cout << messageToClient.toStdString() << std::endl;
            std::cout << "Gegenereerd random getal = " << randomNumber << std::endl;
            std::cout << std::endl;

            // Send the message to the client
            pushSocket->send(messageToClient.toStdString().c_str(), messageToClient.length());

            std::cout << "Random Number gesaved in random_numbers.csv" << std::endl;
            std::cout << std::endl;
            std::cout << "------------------------------------------------------------------------" << std::endl;
            std::cout << std::endl;

            getQuestion();
        }
    }
    // Catch exceptions
    catch (zmq::error_t &ex)
    {
        std::cerr << "Uitzondering gevonden : " << ex.what();
    }
}

std::string service::replaceAnsWithValue(const std::string &formula)
{
    std::string replacedFormula = formula;

    size_t pos = replacedFormula.find("ans");
    while (pos != std::string::npos)
    {
        // Replace "ans" with the stored value
        std::string ansStr = std::to_string(ans);
        replacedFormula.replace(pos, 3, ansStr);

        pos = replacedFormula.find("ans", pos + ansStr.length());
    }

    return replacedFormula;
}

void service::setAns(double value)
{
    ans = value;
}
