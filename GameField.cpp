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
    barriers.clear();
    rocketBit.setPos(5.0, 8.0);
    ball.setPos(5.0, 6.0);
}


void GameField::loadLevel(int levelNum)
{
    clearLevel();

    if(levelNum==1){
        // Левая стена
        Barrier *barrierLeft=new Barrier;
        barrierLeft->setPos(0.0, 0.0);
        barriers.append( barrierLeft ); // Запоминается указатель на препятствие
        this->addItem(barrierLeft); // Препятствие кладется на поле

        // Правая стена
        Barrier *barrierRight=new Barrier;
        barrierRight->setPos(9.0, 0.0);
        barriers.append( barrierRight );
        this->addItem(barrierRight);

        // Верхняя стена
        Barrier *barrierTop=new Barrier;
        barrierTop->setPos(0.0, 0.0);
        barriers.append( barrierTop );
        this->addItem(barrierTop);

        // Кирпич
        Barrier *brick=new Barrier;
        brick->setPos(0.0, 0.0);
        barriers.append( brick );
        this->addItem(brick);
    }
}
