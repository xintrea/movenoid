#include "MoveDetector.h"
#include "AppConfig.h"

#include <QPoint>

extern AppConfig appConfig;

MoveDetector::MoveDetector()
{
    // Инициализируется устройство захвата изображения
    captureDevice.init( appConfig.getParameter("captureDeviceFileName") );
    captureDevice.setBrigthnessThreshold( appConfig.getParameter("brigthnessThreshold").toInt() );
}


// Координаты ракетки
QPoint MoveDetector::getRocketBitPos()
{
    return getFakeRocketBitPos();

}


// Наклон ракетки
qreal MoveDetector::getRocketBitAngle()
{
    return getFakeRocketBitAngle();
}


QPoint MoveDetector::getFakeRocketBitPos()
{
    static qreal angleShiftX=0.0;
    static qreal angleShiftY=0.0;

    angleShiftX+=0.05;
    angleShiftY+=0.025;

    qreal x=5.0+sin(angleShiftX)*4.0;
    qreal y=8.0+sin(angleShiftX)*1.5;

    return QPoint(x, y);
}


qreal MoveDetector::getFakeRocketBitAngle()
{
    static qreal angle=0.0;
    static int direction=1;
    qreal delta=0.01;
    qreal maxAngle=0.5;

    angle=angle+(qreal)direction*delta;

    if(fabs(angle)>maxAngle)
        direction=-direction;

    return angle;
}

