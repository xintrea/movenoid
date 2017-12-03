#include <math.h>

#include "MathAn.h"

MathAn::MathAn()
{

}


// Преобразование радианы в градусы
qreal MathAn::radToDeg(const qreal rad)
{
    return (rad * 180.0) / PI;
}


// Преобразование градусы в радианы
qreal MathAn::degToRad(const qreal deg)
{
    return (PI * deg) / 180.0;
}


// Функция преобразует угол angle с шагом 90 градусов так, чтобы он
// примерно соответствовал углу wayMarkAngle, на который он должен ориентироваться
qreal MathAn::selectNearestAngle(qreal angle, const qreal wayMarkAngle, const qreal angleDispersion)
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


// Определениу угла по двум точкам
qreal MathAn::getAngleByPoints(const qreal xA, const qreal yA, const qreal xB, const qreal yB)
{
    // qreal middleAngle=(marker.angleA+marker.angleB)/2.0;
    qreal angle=radToDeg( atan2( -(xA-xB), yA-yB) );
    angle-=90.0;
    if(angle < 0.0)
        angle = 360.0 + angle;

    return angle;
}
