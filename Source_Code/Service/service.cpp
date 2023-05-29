#include "service.h"
#include <iostream>
#include <string>

void service::makeHttpRequest(QString Formule)
{
    qInfo() << "Formule =" << Formule;

    QString Buffer = "https://api.mathjs.org/v4/?expr=";
    Buffer += QUrl::toPercentEncoding( Formule );
    QUrl url(Buffer);
    qInfo() << url;
    QNetworkRequest request(url);

    QNetworkReply* reply = networkManager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        QByteArray responseData = reply->readAll();

        qInfo() << "Answer =" << responseData;
        // Do something with responseData
        reply->deleteLater();
        //networkManager->deleteLater();

        // terugsturen naar Client
        QString Message_To_Client = PushTopicCalculator.c_str() + responseData;
        pushSocket->send(Message_To_Client.toStdString().c_str(), Message_To_Client.length());

        getQuestion();
    });
}

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

            // logic
            std::string Buffer = GetPushTopicCalculator();
            std::string output = message.substr(SubscribeTopicCalculator.length()); //message.find(">", SubscribeTopic.length()));
            // std::cout << output << std::endl;

            makeHttpRequest (output.c_str());
            /* extract numbers and symbol from input string
            // int num1 = stoi(output.substr(0, output.find("+")));
            // int num2 = stoi(output.substr(output.find("+") + 1, output.length()));
            // char symbol = '+';

            // perform arithmetic operation based on symbol
            // int result = 0;
            // switch (symbol) {
            //    case '+':
            //        result = num1 + num2;
            //        break;
            //    case '-':
            //        result = num1 - num2;
            //        break;
            //    case '*':
            //        result = num1 * num2;
            //        break;
            //    case '/':
            //        result = num1 / num2;
            //        break;
            //   default:
            //        std::cout << "Ongeldige bewerking." << std::endl;
            // }

            // Buffer.append(std::to_string(result));
            // pushSocket->send(Buffer.c_str(), Buffer.length());

           // std::cout << output << std::endl; */
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Uitzondering gevonden : " << ex.what();
    }
}
