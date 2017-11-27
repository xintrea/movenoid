#ifndef MOVEDETECTOR_H
#define MOVEDETECTOR_H

#include <QPointF>
#include "opencv2/opencv.hpp"
#include "CaptureDevice.h"

struct Marker{

    int chunks; // Количество частей
                // 0 - нет маркера
                // 1 - одна большая часть (10:3) - для движущегося смазанного и потому "слипшегося" изображения
                // 2 - две части (1:1 и 2:1) - для четкого изображения
    QList<QPoinF> verticlesA;
    QList<QPoinF> verticlesB;

    qfloat massCenterA;
    qfloat massCenterB;

    qfloat angleA;
    qfloat angleB;

    QSizeF sizeA;
    QSizeF sizeB;

};


class MoveDetector
{
public:
    MoveDetector();
    QPointF getRocketBitPos();
    qreal getRocketBitAngle();
    Marker detectMarker();

protected:
    QPointF getFakeRocketBitPos();
    qreal getFakeRocketBitAngle();

    qreal rocetBitX;
    qreal rocetBitY;
    qreal rocetBitAngle;

    CaptureDevice captureDevice;

};

#endif // MOVEDETECTOR_H
