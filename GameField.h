#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QGraphicsScene>
#include <QList>
#include <Box2D/Box2D.h>

#include "Barrier.h"
#include "RocketBit.h"
#include "Ball.h"

class GameField : public QGraphicsScene
{
public:
    GameField();

protected:
    // QList<Barrier> Barriers;
    Barrier barriers;
    RocketBit rocketBit;
    Ball ball;

    b2World *physicsWorld;
};

#endif // GAMEFIELD_H
