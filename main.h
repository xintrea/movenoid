#ifndef MAIN_H
#define MAIN_H

#include <QString>
#include "opencv2/opencv.hpp"

// Версия программы
#define APPLICATION_RELEASE_VERSION         0
#define APPLICATION_RELEASE_SUBVERSION      1
#define APPLICATION_RELEASE_MICROVERSION    0

#define MOVE_NOID_START_BALL_POS_X 5.0
#define MOVE_NOID_START_BALL_POS_Y 7.0
#define MOVE_NOID_START_BALL_VELOCITY_X -2.0
#define MOVE_NOID_START_BALL_VELOCITY_Y -6.0

#define MOVE_NOID_START_ROCKET_BIT_POS_X 5.0
#define MOVE_NOID_START_ROCKET_BIT_POS_Y 8.5

void criticalError(const QString message);

#endif // MAIN_H
