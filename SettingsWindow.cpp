#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QLabel>

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
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}


void SettingsWindow::init(void)
{
    // На экране устанавливается значение файла устрофства захвата, считанное из конфига
    ui->captureDeviceFileName->setText(appConfig.getParameter("captureDeviceFileName"));

    // На экране устанавливается положение движка границы отсечения яркости
    ui->brigthnessTrashholdSlider->setMinimum(0);
    ui->brigthnessTrashholdSlider->setMaximum(100);
    ui->brigthnessTrashholdSlider->setSliderPosition(appConfig.getParameter("brigthnessTrashhold").toInt());

    cv::VideoCapture cap;

    if(ui->captureDeviceFileName->text()=="default") {
        cap.open(0); // Default камера
        if(!cap.isOpened()) {
            QMessageBox msgBox;
            msgBox.setText("Can't open default camera");
            msgBox.exec();
            return;
        }
    } else {
        cap.open( ui->captureDeviceFileName->text().toLocal8Bit().constData() );
        if(!cap.isOpened()) {
            QMessageBox msgBox;
            msgBox.setText("Can't open camera device file: "+ui->captureDeviceFileName->text());
            msgBox.exec();
            return;
        }
    }

    // Из потока берется один первый кадр для определения параметров сохраняемого видео
    cv::Mat src;
    cap >> src;
    if (src.empty()) {
        QMessageBox msgBox;
        msgBox.setText("Error. Blank frame grabbed.");
        msgBox.exec();
    }
    cv::Size sizeOfFrame=src.size();
    qDebug() << "Capture W: " << sizeOfFrame.width << " H: " << sizeOfFrame.height;

    // QImage img((uchar*)src.data, src.cols, src.rows, src.step, QImage::Format_RGB32);
    QImage img((uchar*)src.data, src.cols, src.rows, src.step, QImage::Format_RGB888);
    cv::imshow("Source frame", src);

    /*
    QGraphicsScene *scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(QRect(0,0, sizeOfFrame.width, sizeOfFrame.height));
    QGraphicsItem * sceneItem = scene->addPixmap(QPixmap::fromImage( img ));
    sceneItem->setPos(QPoint(10, 10));
    */

    ui->graphicsPixmapLabel->setMinimumSize(sizeOfFrame.width, sizeOfFrame.height);
    ui->graphicsPixmapLabel->setPixmap(QPixmap::fromImage( img ));

}


// Клик по кнопке Reset to default
void SettingsWindow::onCaptureDeviceDefaultButtonClicked()
{
    ui->captureDeviceFileName->setText("default");
    appConfig.setParameter("captureDeviceFileName", "default");

}


// Клик по кнопке Apply
void SettingsWindow::onCaptureDeviceApplyButtonClicked()
{

}
