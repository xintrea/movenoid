#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QImage>

#include <iostream>

#include "MainWindow.h"
#include "AppConfig.h"

#include "main.h"


using namespace std;

// Указатель на основное окно программы
QObject *pMainWindow;

// Конфигурация программы (считанная из файла конфигурации)
AppConfig appConfig;

void criticalError(QString message)
{
    qDebug() << " ";
    qDebug() << "---------------";
    qDebug() << "Critical error!";
    qDebug() << "---------------";
    qDebug() << message;
    qDebug() << "---------------";
    qDebug() << " ";

    QMessageBox::critical(qobject_cast<QWidget *>(pMainWindow), "Critical error",
                        message+"\n\nProgramm was closed.",
                        QMessageBox::Ok);

    exit(1);
}


QImage Mat2QImage(const cv::Mat3b &src)
{
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y) {
                const cv::Vec3b *srcrow = src[y];
                QRgb *destrow = (QRgb*)dest.scanLine(y);
                for (int x = 0; x < src.cols; ++x) {
                        destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
                }
        }
        return dest;
}


QImage Mat2QImage(const cv::Mat_<double> &src)
{
        double scale = 255.0;
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y) {
                const double *srcrow = src[y];
                QRgb *destrow = (QRgb*)dest.scanLine(y);
                for (int x = 0; x < src.cols; ++x) {
                        unsigned int color = srcrow[x] * scale;
                        destrow[x] = qRgba(color, color, color, 255);
                }
        }
        return dest;
}


int main(int argc, char *argv[])
{
    qDebug() << "MoveNoid start...";

    QApplication a(argc, argv);
    MainWindow w;
    pMainWindow=&w;
    w.show();

    return a.exec();
}
