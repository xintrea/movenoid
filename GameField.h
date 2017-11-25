#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QObject>
#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include <Box2D/Box2D.h>

#include "Barrier.h"
#include "Brick.h"
#include "RocketBit.h"
#include "Ball.h"

class GameField : public QGraphicsScene
{
    Q_OBJECT

public:
    GameField(QObject *parent = Q_NULLPTR);
    ~GameField();

    void clearLevel();
    void loadLevel(int levelNum);

    void runGame();

private slots:
    void updateWorld();

protected:
    QList<Barrier*> barriers;
    QList<Brick*> bricks;
    RocketBit rocketBit;
    Ball ball;

    b2World *physicsWorld;

    QTimer updateWorldTimer;
};

#endif // GAMEFIELD_H
