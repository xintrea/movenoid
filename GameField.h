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
    void loadLevel(const int levelNum);

    void runGame();

private slots:
    void updateWorld();

signals:
    void setScore(int iScore);
    void setLives(int iLives);
    void closeGame();

protected:
    void createBrick(const qreal x, const qreal y);
    void destroyBricks();

    void checkBallPosition(); // Проверка позиции мяча на предмет выхода его за игровое поле
    void checkBricksCount(); // Проверка количества оставшихся кирпичей

    void initBarriers();
    void initBall();
    void initRocketBit();

    QList<Barrier*> barriers;
    QList<Brick*> bricks;
    RocketBit rocketBit;
    Ball ball;

    b2World *physicsWorld;
    ContactListener *contactListener;

    QTimer updateWorldTimer;

    QThread moveDetectorThread;
    MoveDetector moveDetector;

    int score;
    int lives;
    int level;
};

#endif // GAMEFIELD_H
