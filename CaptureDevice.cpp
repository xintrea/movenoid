#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QDateTime>

#include "CaptureDevice.h"


CaptureDevice::CaptureDevice()
{
    deviceFileName="default";
    brigthnessThreshold=50;
}


// Инициализация устройства видеозахвата
void CaptureDevice::init(const QString iDeviceFileName)
{
    qDebug() << "Start capture device init time: "<< QDateTime::currentDateTime();

    deviceFileName=iDeviceFileName;

    if(iDeviceFileName=="default") {
        videoCaptureFlow.open(0); // Default камера
        if(!videoCaptureFlow.isOpened()) {
            QMessageBox msgBox;
            msgBox.setText("Can't open default camera");
            msgBox.exec();
            return;
        }
    } else {
        videoCaptureFlow.open( iDeviceFileName.toLocal8Bit().constData() ); // Камера на указанном файле устройства
        if(!videoCaptureFlow.isOpened()) {
            QMessageBox msgBox;
            msgBox.setText("Can't open camera device file: "+deviceFileName);
            msgBox.exec();
            return;
        }
    }

    qDebug() << "Finish capture device init time: "<< QDateTime::currentDateTime();
}


void CaptureDevice::setBrigthnessThreshold(const int iBrigthnessThreshold)
{
    brigthnessThreshold=iBrigthnessThreshold;
}


QSize CaptureDevice::getFrameSize(void)
{
    return QSize(videoCaptureFlow.get(CV_CAP_PROP_FRAME_WIDTH), videoCaptureFlow.get(CV_CAP_PROP_FRAME_HEIGHT));
}


cv::Mat* CaptureDevice::getBwFrame(void)
{
    // Из потока берется один кадр
    videoCaptureFlow >> currentFrame;
    if (currentFrame.empty())
        return nullptr;

    double thresholdLevel=(double) brigthnessThreshold;

    // Преобразование в Ч/Б и превращение в контрастное изображение с порогом brigthnessThreshold
    cv::cvtColor(currentFrame, currentBwFrame, CV_BGR2GRAY);
    cv::GaussianBlur(currentBwFrame, currentBwFrame, cv::Size(7,7), 1.5, 1.5);
    cv::threshold(currentBwFrame, currentBwFrame, thresholdLevel, 254.0, cv::THRESH_BINARY);
    cv::flip(currentBwFrame, currentBwFrame, 1);

    // cv::imshow("Source frame", currentBwFrame);

    return &currentBwFrame;
}
