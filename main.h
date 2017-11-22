#ifndef MAIN_H
#define MAIN_H

#include <QString>
#include "opencv2/opencv.hpp"

// Версия программы
#define APPLICATION_RELEASE_VERSION         0
#define APPLICATION_RELEASE_SUBVERSION      1
#define APPLICATION_RELEASE_MICROVERSION    0


QImage Mat2QImage(const cv::Mat3b &src);
QImage Mat2QImage(const cv::Mat_<double> &src);

void criticalError(QString message);

#endif // MAIN_H
