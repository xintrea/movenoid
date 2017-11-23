#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QLabel>
#include <QProgressDialog>
#include <QDateTime>
#include <QTimer>

#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"
#include "AppConfig.h"
#include "main.h"

extern AppConfig appConfig;

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    init();

    // Таймер обновления картинки
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerFinish()));
    timer->start(50);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
    delete timer;
}


void SettingsWindow::init(void)
{
    // На экране устанавливается значение файла устройства захвата, считанное из конфига
    ui->captureDeviceFileName->setText(appConfig.getParameter("captureDeviceFileName"));

    // На экране устанавливается положение движка границы отсечения яркости
    ui->brigthnessThresholdSlider->setMinimum(0);
    ui->brigthnessThresholdSlider->setMaximum(254);
    ui->brigthnessThresholdSlider->setSliderPosition(appConfig.getParameter("brigthnessThreshold").toInt());

    initCaptureDevice();
    updateCaptureImage(); // Устанавливается первая картинка с камеры
}


void SettingsWindow::initCaptureDevice(void)
{
    qDebug() << "Start init time: "<< QDateTime::currentDateTime();

    if(ui->captureDeviceFileName->text()=="default") {
        captureDevice.open(0); // Default камера
        if(!captureDevice.isOpened()) {
            QMessageBox msgBox;
            msgBox.setText("Can't open default camera");
            msgBox.exec();
            return;
        }
    } else {
        captureDevice.open( ui->captureDeviceFileName->text().toLocal8Bit().constData() );
        if(!captureDevice.isOpened()) {
            QMessageBox msgBox;
            msgBox.setText("Can't open camera device file: "+ui->captureDeviceFileName->text());
            msgBox.exec();
            return;
        }
    }

    qDebug() << "Finish init time: "<< QDateTime::currentDateTime();
}


QImage SettingsWindow::getCurrentImage(void)
{
    // qDebug() << "Start getCurrentImage time: "<< QDateTime::currentDateTime();

    // Из потока берется один кадр
    captureDevice >> currentFrame;
    if (currentFrame.empty())
        return QImage();
    // cv::imshow("Source frame", src);

    // cv::Size sizeOfFrame=currentFrame.size();
    // qDebug() << "Capture W: " << sizeOfFrame.width << " H: " << sizeOfFrame.height;

    /*
    cv::flip(currentFrame, currentFrame, 1);
    QImage img((uchar*)currentFrame.data, currentFrame.cols, currentFrame.rows, currentFrame.step, QImage::Format_RGB888); // Цветное изображение
    img=img.rgbSwapped(); // Преобразование цветов из BGR (OpenCV) в RGB (Qt)
    */

    double thresholdLevel=(double) ui->brigthnessThresholdSlider->sliderPosition();

    cv::cvtColor(currentFrame, currentBwFrame, CV_BGR2GRAY);
    cv::GaussianBlur(currentBwFrame, currentBwFrame, cv::Size(7,7), 1.5, 1.5);
    cv::threshold(currentBwFrame, currentBwFrame, thresholdLevel, 254.0, cv::THRESH_BINARY);
    cv::flip(currentBwFrame, currentBwFrame, 1);

    // QImage img;
    QImage img((uchar*)currentBwFrame.data, currentBwFrame.cols, currentBwFrame.rows, currentBwFrame.step, QImage::Format_Grayscale8);

    // cv::imshow("Source frame", currentBwFrame);
    // qDebug() << "Finish getCurrentImage time: "<< QDateTime::currentDateTime();

    return img;
}


void SettingsWindow::updateCaptureImage(void)
{
    int enabledWidth=ui->mainVerticalLayout->sizeHint().width();
    ui->graphicsPixmapLabel->setPixmap(QPixmap::fromImage( getCurrentImage() ).scaledToWidth(enabledWidth, Qt::FastTransformation));
}


// Слот, срабатывающий по таймеру
void SettingsWindow::onTimerFinish(void)
{
    updateCaptureImage();
    // qDebug() << "Image update time: "<< QDateTime::currentDateTime();
}


// Клик по кнопке Reset to default
void SettingsWindow::onCaptureDeviceDefaultButtonClicked()
{
    ui->captureDeviceFileName->setText("default");
    appConfig.setParameter("captureDeviceFileName", "default");

    timer->stop();
    initCaptureDevice();
    timer->start();
}


// Клик по кнопке Apply
void SettingsWindow::onCaptureDeviceApplyButtonClicked()
{
    QString deviceFileName=ui->captureDeviceFileName->text();
    appConfig.setParameter("captureDeviceFileName", deviceFileName);

    timer->stop();
    initCaptureDevice();
    timer->start();
}


void SettingsWindow::onBrigthnessThresholdSliderMoved(int position)
{
    qDebug() << "In onBrigthnessThresholdSliderMoved slot";

    QString n=QString::number( position );
    appConfig.setParameter("brigthnessThreshold", n);
}
