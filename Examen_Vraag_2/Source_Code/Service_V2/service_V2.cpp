#include "service_V2.h"

#include <iostream>
#include <string>
#include <fstream>

#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

service_V2::service_V2()
{
    subSocket->setsockopt(ZMQ_SUBSCRIBE, SubscribeTopicCalculator.c_str(), SubscribeTopicCalculator.length());
    subSocket->setsockopt(ZMQ_SUBSCRIBE, SubscribeTopicRandomNumber.c_str(), SubscribeTopicRandomNumber.length());
    subSocket->setsockopt(ZMQ_SUBSCRIBE, SubscribeTopicStore.c_str(), SubscribeTopicStore.length());

    subSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");
    pushSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");

    networkManager = new QNetworkAccessManager();
}

void service_V2::getQuestion()
{
    try
    {
        if (subSocket->connected())
        {
            subSocket->recv(zmqBuffer);

            std::string message((char *)zmqBuffer->data(), zmqBuffer->size());

            QString messageQstring = message.c_str();
            const QStringList parsedBuffer = messageQstring.split(">");
            const QString topic = parsedBuffer.value(2);
            const QString userId = parsedBuffer.value(3);

            if (topic == "calculator")
            {
                std::string Buffer = GetPushTopicCalculator() + userId.toStdString();
                std::string output = message.substr(Buffer.length() + 1);

                output = replaceAnsWithValue(output);

                makeHttpRequest(output.c_str(), userId);
            }
            else if (topic == "randomnumber")
            {
                generateRandomNum(userId);
            }
            else if (topic == "store")
            {
                std::string storeId = parsedBuffer.value(4).toStdString();
                storeLastAnswer(storeId);
            }
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Uitzondering gevonden : " << ex.what();
    }
}

void service_V2::makeHttpRequest(QString Formula, QString userId)
{
    qInfo() << "Formule =" << Formula;

    QString Buffer = "https://api.mathjs.org/v4/?expr=";
    Buffer += QUrl::toPercentEncoding(Formula);
    QUrl url(Buffer);
    qInfo() << url;
    QNetworkRequest request(url);

    QNetworkReply *reply = networkManager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        QByteArray responseData = reply->readAll();
        qDebug() << responseData;

        if (responseData.startsWith("Error"))
        {
            std::cout << "API Error: " << std::endl;
        }
        else
        {
            std::cout << "test1" << std::endl;
            qDebug() << responseData;
            ans = responseData.toDouble();
            std::cout << ans << std::endl;

            std::string formulaStdString = Formula.toStdString();
            formulaStdString = replaceStoreValues(formulaStdString);
            QString convertedFormula = QString::fromStdString(formulaStdString);
        }

        std::cout << std::endl;
        qInfo() << "Answer =" << responseData;
        std::cout << std::endl;

        reply->deleteLater();

        QString messageToClient = PushTopicCalculator.c_str() + userId + ">" + responseData;

        std::cout << userId.toStdString() << std::endl;
        std::cout << messageToClient.toStdString() << std::endl;

        pushSocket->send(messageToClient.toStdString().c_str(), messageToClient.length());

        std::ofstream outputFile("Formules_en_Antwoorden.csv", std::ios::app);

        if (!outputFile)
        {
            std::cerr << "Error bij het openen van de file." << std::endl;
        }

        outputFile << Formula.toStdString() << std::endl;
        outputFile << responseData.toStdString() << std::endl;

        std::cout << "Formule en antwoord gesaved in Formules_en_Antwoorden.csv" << std::endl;
        std::cout << std::endl;
        std::cout << "------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;

        getQuestion();
    });
}

void service_V2::generateRandomNum(QString userId)
{
    try
    {
        if (subSocket->connected())
        {
            int randomNumber = std::rand() % 101;

            std::ofstream outputFile("random_numbers.csv", std::ios::app);

            if (!outputFile)
            {
                std::cerr << "Error bij het openen van de file." << std::endl;
            }

            outputFile << randomNumber << std::endl;

            QString messageToClient = PushTopicRandomNumber.c_str() + userId + ">";
            messageToClient.append(std::to_string(randomNumber).c_str());

            std::cout << messageToClient.toStdString() << std::endl;
            std::cout << "Gegenereerd random getal = " << randomNumber << std::endl;
            std::cout << std::endl;

            pushSocket->send(messageToClient.toStdString().c_str(), messageToClient.length());

            std::cout << "Random Number gesaved in random_numbers.csv" << std::endl;
            std::cout << std::endl;
            std::cout << "------------------------------------------------------------------------" << std::endl;
            std::cout << std::endl;

            getQuestion();
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Uitzondering gevonden : " << ex.what();
    }
}

std::string service_V2::replaceAnsWithValue(const std::string &formula)
{
    std::string replacedFormula = formula;

    size_t pos = replacedFormula.find("ans");
    while (pos != std::string::npos)
    {
        std::string ansStr = std::to_string(ans);
        replacedFormula.replace(pos, 3, ansStr);

        pos = replacedFormula.find("ans", pos + ansStr.length());
    }

    return replacedFormula;
}

void service_V2::setAns(double value)
{
    ans = value;
}

void service_V2::storeLastAnswer(const std::string &storeId)
{
    std::ofstream outputFile("Formules_en_Antwoorden.csv", std::ios::app);

    if (!outputFile)
    {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::string lastAnswer = std::to_string(ans);

    outputFile << storeId << "," << lastAnswer << std::endl;

    std::cout << "Last answer stored in Formules_en_Antwoorden.csv with ID " << storeId << std::endl;
    std::cout << std::endl;
    std::cout << "Ma" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    outputFile.close();

    getQuestion();
}

std::string service_V2::replaceStoreValues(const std::string &formula)
{
    std::string replacedFormula = formula;

    size_t pos = replacedFormula.find("Ma");
    if (pos != std::string::npos)
    {
        std::string value = getStoreValue("Ma");
        replacedFormula.replace(pos, 2, value);
    }

    pos = replacedFormula.find("Mb");
    if (pos != std::string::npos)
    {
        std::string value = getStoreValue("Mb");
        replacedFormula.replace(pos, 2, value);
    }

    pos = replacedFormula.find("Mc");
    if (pos != std::string::npos)
    {
        std::string value = getStoreValue("Mc");
        replacedFormula.replace(pos, 2, value);
    }

    return replacedFormula;
}

std::string service_V2::getStoreValue(const std::string &storeId)
{
    std::ifstream inputFile("Formules_en_Antwoorden.csv");
    std::string line;

    if (!inputFile)
    {
        std::cerr << "Error opening file." << std::endl;
        return "";
    }

    while (std::getline(inputFile, line))
    {
        std::string::size_type commaPos = line.find(",");
        if (commaPos != std::string::npos)
        {
            std::string id = line.substr(0, commaPos);
            if (id == storeId)
            {
                std::string value = line.substr(commaPos + 1);
                return value;
            }
        }
    }

    return "";
}
