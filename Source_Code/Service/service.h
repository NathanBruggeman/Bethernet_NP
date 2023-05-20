#ifndef SERVICE_H
#define SERVICE_H

#include <iostream>
#include <zmq.hpp>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n) Sleep(n)
#endif

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <QDebug>

class service
{
public:
    service();
    std::string GetPushTopic(void)
    {
        return PushTopic;
    }
    std::string GetSubTopic(void)
    {
        return SubscribeTopic;
    }

    void makeHttpRequest(QString Formule);
    void getQuestion();

private:
    std::string SubscribeTopic = "Nathan>service?>";
    std::string PushTopic = "Nathan>service!>";
    zmq::context_t *context = new zmq::context_t(1);
    zmq::message_t *zmqBuffer = new zmq::message_t();
    zmq::socket_t *pushSocket = new zmq::socket_t(*context, ZMQ_PUSH);
    zmq::socket_t *subSocket = new zmq::socket_t(*context, ZMQ_SUB);
    QNetworkAccessManager* networkManager;
    /* zmq::context_t *context = new zmq::context_t(1);
    zmq::socket_t *socket = new zmq::socket_t ( *context, ZMQ_SUB );
    zmq::socket_t *Push = new zmq::socket_t ( *context, ZMQ_PUSH ); */
};

#endif // SERVICE_H
