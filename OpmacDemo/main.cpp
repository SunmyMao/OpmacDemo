#include "opmac_demo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpmacDemo w;
    w.show();
    return a.exec();
}
