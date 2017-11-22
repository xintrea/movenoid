#include <QApplication>
#include <QDebug>
#include <iostream>
#include "MainWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    qDebug() << "MoveNoid start...";

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
