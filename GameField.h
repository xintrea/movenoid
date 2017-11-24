#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QGraphicsScene>
#include <QList>
#include <Box2D/Box2D.h>

#include "Barrier.h"
#include "Brick.h"
#include "RocketBit.h"
#include "Ball.h"

class GameField : public QGraphicsScene
{
public:
    GameField();
    virtual ~GameField();

    void clearLevel();
    void loadLevel(int levelNum);

    void runGame();

protected:
    QList<Barrier*> barriers;
    QList<Brick*> bricks;
    RocketBit rocketBit;
    Ball ball;

    b2World *physicsWorld;
};

#endif // GAMEFIELD_H
