#ifndef SERVICE_V2_H
#define SERVICE_V2_H

#include <iostream>
#include <zmq.hpp>
#include <string>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <QDebug>

class service_V2
{
public:
    service_V2();

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

    std::string GetPushTopicStore(void)
    {
        return PushTopicStore;
    }
    std::string GetSubTopicStore(void)
    {
        return SubscribeTopicStore;
    }

    void makeHttpRequest(QString Formula, QString userId);
    void getQuestion();
    void generateRandomNum(QString userId);
    std::string replaceAnsWithValue(const std::string &formula);
    void setAns(double value);
    void storeLastAnswer(const std::string &storeId);
    std::string replaceStoreValues(const std::string &formula);
    std::string getStoreValue(const std::string &storeId);

private:
    std::string SubscribeTopicCalculator = "Nathan>service?>calculator>";
    std::string PushTopicCalculator = "Nathan>service!>calculator>";
    std::string SubscribeTopicRandomNumber = "Nathan>service?>randomnumber>";
    std::string PushTopicRandomNumber = "Nathan>service!>randomnumber>";
    std::string SubscribeTopicStore = "Nathan>service?>store>";
    std::string PushTopicStore = "Nathan>service!>store>";

    zmq::context_t *context = new zmq::context_t(1);
    zmq::message_t *zmqBuffer = new zmq::message_t();
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subSocket = new zmq::socket_t(*context, ZMQ_SUB);

    QNetworkAccessManager *networkManager;

    double ans;
};

#endif // SERVICE_V2_H
