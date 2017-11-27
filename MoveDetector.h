#ifndef MOVEDETECTOR_H
#define MOVEDETECTOR_H

#include <QPointF>
#include "opencv2/opencv.hpp"
#include "CaptureDevice.h"

class MoveDetector
{
public:
    MoveDetector();
    QPointF getRocketBitPos();
    qreal getRocketBitAngle();

protected:
    QPointF getFakeRocketBitPos();
    qreal getFakeRocketBitAngle();

    qreal rocetBitX;
    qreal rocetBitY;
    qreal rocetBitAngle;

    CaptureDevice captureDevice;

};

#endif // MOVEDETECTOR_H
