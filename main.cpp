#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include <iostream>

#include "MainWindow.h"
#include "AppConfig.h"

using namespace std;

// Указатель на основное окно программы
QObject *pMainWindow;

// Конфигурация программы (считанная из файла конфигурации)
AppConfig appConfig;

void criticalError(QString message)
{
    qDebug() << " ";
    qDebug() << "---------------";
    qDebug() << "Critical error!";
    qDebug() << "---------------";
    qDebug() << message;
    qDebug() << "---------------";
    qDebug() << " ";

    QMessageBox::critical(qobject_cast<QWidget *>(pMainWindow), "Critical error",
                        message+"\n\nProgramm was closed.",
                        QMessageBox::Ok);

    exit(1);
}


int main(int argc, char *argv[])
{
    qDebug() << "MoveNoid start...";

    QApplication a(argc, argv);
    MainWindow w;
    pMainWindow=&w;
    w.show();

    return a.exec();
}
