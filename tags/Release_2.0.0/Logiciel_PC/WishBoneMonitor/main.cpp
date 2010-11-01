#include <QtGui/QApplication>
#include "mainwindow.h"
#include "wishbonemonitor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WishBoneMonitor WBM;

    MainWindow w(&WBM);
    w.show();

    return a.exec();
}
