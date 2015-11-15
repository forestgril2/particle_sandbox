#include <QtGui/QApplication>
#include "lennardNet.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    lennardNet lennardnet;
    lennardnet.show();
    return app.exec();
}
