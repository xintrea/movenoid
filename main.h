#ifndef MAIN_H
#define MAIN_H

#include <QString>
#include "opencv2/opencv.hpp"

// Версия программы
#define APPLICATION_RELEASE_VERSION         0
#define APPLICATION_RELEASE_SUBVERSION      1
#define APPLICATION_RELEASE_MICROVERSION    0

#define PI 3.14159265358979323846264338327950288419717

qreal radToDeg(qreal rad);
qreal degToRad(qreal deg);
qreal selectNearestAngle(qreal angle, qreal wayMarkAngle, qreal angleDispersion);
qreal getAngleByPoints(const qreal xA, const qreal yA, const qreal xB, const qreal yB);

void criticalError(QString message);


#endif // MAIN_H
