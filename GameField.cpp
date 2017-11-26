#include "GameField.h"

GameField::GameField(QObject *parent) : QGraphicsScene(parent)
{
    connect(&updateWorldTimer, SIGNAL(timeout()), this, SLOT(updateWorld()));

    this->setSceneRect(0.0, 0.0, 10.0, 10.0);

    b2Vec2 gravity(0.0, 5.0);
    physicsWorld=new b2World(gravity);

    loadLevel(1);
}

GameField::~GameField()
{
    clearLevel();
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
    foreach( Brick *currentBrick, bricks ) {
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
        Barrier *barrierLeft=new Barrier();
        polygon << QPointF(0.0, 0.0) << QPointF(0.0, 10.0) << QPointF(0.05, 10.0) << QPointF(0.05, 0.0);
        barrierLeft->setPolygon(polygon);
        barrierLeft->setPos(0.0, 0.0);
        barriers.append( barrierLeft ); // Запоминается указатель на препятствие
        this->addItem(barrierLeft); // Препятствие кладется на поле
        barrierLeft->setPhysicsWorld(physicsWorld);

        // Правая стена
        Barrier *barrierRight=new Barrier();
        barrierRight->setPolygon(polygon);
        barrierRight->setPos(10.0-0.049, 0.0);
        barriers.append( barrierRight );
        this->addItem(barrierRight);
        barrierRight->setPhysicsWorld(physicsWorld);

        // Верхняя стена
        Barrier *barrierTop=new Barrier();
        polygon.clear();
        polygon << QPointF(0.0, 0.0) << QPointF(0.0, 0.05) << QPointF(10.0, 0.05) << QPointF(10.0, 0.0);
        barrierTop->setPolygon(polygon);
        barrierTop->setPos(0.0, 0.0);
        barriers.append( barrierTop );
        this->addItem(barrierTop);
        barrierTop->setPhysicsWorld(physicsWorld);

        // Просто квадратик
        Barrier *barrierSquare=new Barrier();
        polygon.clear();
        polygon << QPointF(0.0, 0.0) << QPointF(0.0, 0.15) << QPointF(8.0, 0.15) << QPointF(8.0, 0.0);
        barrierSquare->setRotation(-30.0);
        barrierSquare->setPolygon(polygon);
        barrierSquare->setPos(1.5, 9.5);
        barriers.append( barrierSquare );
        this->addItem(barrierSquare);
        barrierSquare->setPhysicsWorld(physicsWorld);

        // Создание кирпичей
        for(int i=0; i<5; i++) {
            Brick *brick=new Brick;
            brick->setPos((qreal) i*2+1.0, 4.0);
            bricks.append( brick ); // Запоминается указатель на кирпич
            this->addItem(brick); // Кирпич кладется на поле
            brick->setPhysicsWorld(physicsWorld);
        }

        // Установки мячика
        ball.setRadius(0.15);
        // ball.setPos(7.0, 2.0);
        ball.setPos(6.0, 2.0);
        this->addItem(&ball); // Мячик кладется на поле
        ball.setPhysicsWorld(physicsWorld);


    }
}


void GameField::runGame()
{
    updateWorldTimer.start(1000/60);
}


// Слот, срабатывающий по таймеру updateWorldTimer
void GameField::updateWorld()
{
    physicsWorld->Step(1.0/60.0, 6, 2);

    ball.updatePosByPhysicsWorld();

    // Обновляется сцена
    this->update();
}


