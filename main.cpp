#include <QtGui/QApplication>
#include "micestats.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MiceStats w;
    w.show();

    return a.exec();
}
