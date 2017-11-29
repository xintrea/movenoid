#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QImage>

#include <iostream>

#include "MainWindow.h"
#include "AppConfig.h"

#include "main.h"


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


qreal radToDeg(const qreal rad)
{
    return (rad * 180.0) / PI;
}


qreal degToRad(const qreal deg)
{
    return (PI * deg) / 180.0;
}

// Функция преобразует угол angle с шагом 90 градусов так, чтобы он
// примерно соответствовал углу wayMarkAngle, на который он должен ориентироваться
qreal selectNearestAngle(qreal angle, qreal wayMarkAngle, qreal angleDispersion)
{
    // На вход может быть подан отрицательный угол, его надо нормировать
    if(angle < 0.0)   angle+=360.0;
    if(angle > 360.0) angle-=360.0;

    for(int i=0; i<=270; i=i+90) {
        qreal a=angle+(qreal)(i);
        if(a < 0.0)   a+=360.0; // Нормировка угла a
        if(a > 360.0) a-=360.0;
        if(qAbs(a-wayMarkAngle)<angleDispersion)
            return a;
    }

    // Если угол не подобрался по причине того что не уместился в погрешность, он просто возвращается
    return angle;
}


qreal getAngleByPoints(const qreal xA, const qreal yA, const qreal xB, const qreal yB)
{
    // qreal middleAngle=(marker.angleA+marker.angleB)/2.0;
    qreal angle=radToDeg( atan2( -(xA-xB), yA-yB) );
    angle-=90.0;
    if(angle < 0.0)
        angle = 360.0 + angle;

    return angle;
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
