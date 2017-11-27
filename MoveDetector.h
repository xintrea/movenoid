#ifndef MOVEDETECTOR_H
#define MOVEDETECTOR_H

#include <QPoint>
#include "opencv2/opencv.hpp"
#include "CaptureDevice.h"

class MoveDetector
{
public:
    MoveDetector();
    QPoint getRocketBitPos();
    qreal getRocketBitAngle();

protected:

    QPoint getFakeRocketBitPos();
    qreal getFakeRocketBitAngle();

    qreal rocetBitX;
    qreal rocetBitY;
    qreal rocetBitAngle;

    CaptureDevice captureDevice;

};

#endif // MOVEDETECTOR_H
