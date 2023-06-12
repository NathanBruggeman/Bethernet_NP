#include <QCoreApplication>

#include "service_V2.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    service_V2 *service = new service_V2();
    service->getQuestion();

    return a.exec();
}
