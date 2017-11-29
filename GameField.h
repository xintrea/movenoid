#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QObject>
#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include <QThread>
#include <Box2D/Box2D.h>

#include "ContactListener.h"
#include "Barrier.h"
#include "Brick.h"
#include "RocketBit.h"
#include "Ball.h"
#include "MoveDetector.h"

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
    void destroyBricks();

    QList<Barrier*> barriers;
    QList<Brick*> bricks;
    RocketBit rocketBit;
    Ball ball;

    b2World *physicsWorld;
    ContactListener *contactListener;

    QTimer updateWorldTimer;

    QThread moveDetectorThread;
    MoveDetector moveDetector;
};

#endif // GAMEFIELD_H
