#include <QtGui/QApplication>
#include "LennardNet.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    LennardNet lennardnet;
    lennardnet.show();
    return app.exec();
}
