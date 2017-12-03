#ifndef MAIN_H
#define MAIN_H

#include <QString>
#include "opencv2/opencv.hpp"

// ----------------------------------------------------------
// MoveNoid - Арканоид с бесконтактным управлением
// Волгодонск, 2017 г.
// Контакты: xintrea@gmail.com, www.webhamster.ru
// Данный исходный код распространяется под лицензиями:
// - GPL v.3
// - Modified BSD
// © С. М. Степанов, 2017
// ----------------------------------------------------------

// Версия программы
#define APPLICATION_RELEASE_VERSION         0
#define APPLICATION_RELEASE_SUBVERSION      2
#define APPLICATION_RELEASE_MICROVERSION    0

#define MOVE_NOID_FIELD_WIDTH  10.0
#define MOVE_NOID_FIELD_HEIGHT 10.0

#define MOVE_NOID_START_BALL_POS_X 5.0
#define MOVE_NOID_START_BALL_POS_Y 7.0
#define MOVE_NOID_START_BALL_VELOCITY_X -2.0
#define MOVE_NOID_START_BALL_VELOCITY_Y -6.0

#define MOVE_NOID_START_ROCKET_BIT_POS_X 5.0
#define MOVE_NOID_START_ROCKET_BIT_POS_Y 8.5

void criticalError(const QString message);

#endif // MAIN_H
