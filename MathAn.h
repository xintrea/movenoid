#ifndef MATHAN_H
#define MATHAN_H

#include <QtGlobal>

#define PI 3.14159265358979323846264338327950288419717

class MathAn
{
public:
    MathAn();

    static qreal radToDeg(const qreal rad);
    static qreal degToRad(const qreal deg);
    static qreal selectNearestAngle(qreal angle, const qreal wayMarkAngle, const qreal angleDispersion);
    static qreal getAngleByPoints(const qreal xA, const qreal yA, const qreal xB, const qreal yB);
};

#endif // MATHAN_H
