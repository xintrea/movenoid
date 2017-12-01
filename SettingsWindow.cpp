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

    // Инициализируется устройство захвата изображения
    captureDevice.init( ui->captureDeviceFileName->text() );
    captureDevice.setBrigthnessThreshold( ui->brigthnessThresholdSlider->sliderPosition() );

    updateCaptureImage(); // Устанавливается первая картинка с камеры
}


QImage SettingsWindow::getCurrentImage(void)
{
    cv::Mat* currentBwFrame=captureDevice.getBwFrame();
    QImage img((uchar*)currentBwFrame->data, currentBwFrame->cols, currentBwFrame->rows, currentBwFrame->step, QImage::Format_Grayscale8);
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
}


// Клик по кнопке Reset to default
void SettingsWindow::onCaptureDeviceDefaultButtonClicked()
{
    QString deviceFileName="default";

    ui->captureDeviceFileName->setText(deviceFileName);
    appConfig.setParameter("captureDeviceFileName", deviceFileName);

    timer->stop();
    captureDevice.init( deviceFileName );
    timer->start();
}


// Клик по кнопке Apply
void SettingsWindow::onCaptureDeviceApplyButtonClicked()
{
    QString deviceFileName=ui->captureDeviceFileName->text();
    appConfig.setParameter("captureDeviceFileName", deviceFileName);

    timer->stop();
    captureDevice.init( deviceFileName );
    timer->start();
}


void SettingsWindow::onBrigthnessThresholdSliderMoved(const int position)
{
    qDebug() << "In onBrigthnessThresholdSliderMoved slot";

    captureDevice.setBrigthnessThreshold( position );

    QString n=QString::number( position );
    appConfig.setParameter("brigthnessThreshold", n);
}
