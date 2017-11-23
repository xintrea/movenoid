#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QString>

#include "main.h"


namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;
    void init(void);
    void initCaptureDevice(void);
    QImage getCurrentImage(void);
    void updateCaptureImage(void);

protected:
    QString deviceFileName;
    cv::VideoCapture captureDevice;
    cv::Mat currentFrame;
    cv::Mat currentBwFrame;

    QTimer *timer;

private slots:
    void onCaptureDeviceDefaultButtonClicked();
    void onCaptureDeviceApplyButtonClicked();
    void onTimerFinish();
    void onBrigthnessThresholdSliderMoved(int position);
};

#endif // SETTINGSWINDOW_H
