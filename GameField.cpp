#include "GameField.h"

GameField::GameField()
{
    b2Vec2 gravity(0.0, 5.0);
    physicsWorld=new b2World(gravity);

    loadLevel(1);
}

GameField::~GameField()
{
    delete physicsWorld;
}


void GameField::clearLevel()
{
    // Удаляются препятствия
    foreach( Barrier *currentBarrier, barriers ) {
        this->removeItem(currentBarrier);
        delete currentBarrier;
    }
    barriers.clear();

    // Удаляются кирпичи
    foreach( Barrier *currentBrick, barriers ) {
        this->removeItem(currentBrick);
        delete currentBrick;
    }
    bricks.clear();

    rocketBit.setPos(5.0, 8.0);
    ball.setPos(5.0, 6.0);
}


void GameField::loadLevel(int levelNum)
{
    clearLevel();

    if(levelNum==1){

        // Создание препятствий
        QPolygonF polygon;

        // Левая стена
        polygon << QPointF(0.0, 0.0) << QPointF(0.0, 10.0) << QPointF(1.0, 10.0) << QPointF(1.0, 0.0);
        Barrier *barrierLeft=new Barrier(polygon);
        barrierLeft->setPos(0.0, 0.0);
        barriers.append( barrierLeft ); // Запоминается указатель на препятствие
        this->addItem(barrierLeft); // Препятствие кладется на поле

        // Правая стена
        Barrier *barrierRight=new Barrier(polygon);
        barrierRight->setPos(9.0, 0.0);
        barriers.append( barrierRight );
        this->addItem(barrierRight);

        // Верхняя стена
        polygon.clear();
        polygon << QPointF(0.0, 0.0) << QPointF(0.0, 1.0) << QPointF(10.0, 1.0) << QPointF(10.0, 0.0);
        Barrier *barrierTop=new Barrier(polygon);
        barrierTop->setPos(0.0, 0.0);
        barriers.append( barrierTop );
        this->addItem(barrierTop);


        // Создание кирпичей
        Brick *brick=new Brick;
        brick->setPos(3.0, 4.0);
        bricks.append( brick ); // Запоминается указатель на кирпич
        this->addItem(brick); // Кирпич кладется на поле
    }
}
