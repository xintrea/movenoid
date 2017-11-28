#ifndef CAPTUREDEVICE_H
#define CAPTUREDEVICE_H

#include <QSize>
#include <QString>
#include "opencv2/opencv.hpp"

class CaptureDevice
{
public:
    CaptureDevice();
    void init(QString iDeviceFileName);
    void setBrigthnessThreshold(int iBrigthnessThreshold);

    QSize getFrameSize(void);

    cv::Mat* getBwFrame(void);

protected:
    QString deviceFileName;
    cv::VideoCapture videoCaptureFlow;
    cv::Mat currentFrame;
    cv::Mat currentBwFrame;

    int brigthnessThreshold;
};

#endif // CAPTUREDEVICE_H
