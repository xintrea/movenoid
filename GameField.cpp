#include <QMessageBox>
#include <QDebug>

#include "main.h"
#include "ReadyPoster.h"
#include "GameOverPoster.h"
#include "GameField.h"

GameField::GameField(QObject *parent) : QGraphicsScene(parent)
{
    // Параметры сцены
    this->setSceneRect(0.0, 0.0, 10.0, 10.0);

    // Инициализация физического движка
    b2Vec2 gravity(0.0, 0.2); // Сила гравитации на игровом поле
    physicsWorld=new b2World(gravity);

    // Инициализация обработчика пересечений
    contactListener=new ContactListener;
    contactListener->setBall(&ball);
    contactListener->setBricks(&bricks);
    physicsWorld->SetContactListener(contactListener);

    // Инициализация определителя положения ракетки (GameField наследуется от QObject, и здесь работает connect)
    connect(&moveDetectorThread, &QThread::started, &moveDetector, &MoveDetector::run); // При старте треда запустится основной цикл в методе run()
    connect(&moveDetector, SIGNAL(finished()), &moveDetectorThread, SLOT(quit())); // Соединение для корректного завершения потока
    moveDetector.moveToThread(&moveDetectorThread); // Определитель положения ракетки переносится в тред
    moveDetectorThread.start(); // Тред запускается, при этом в нем автоматически будет запущен объект moveDetector

    // Таймер обновления сцены
    connect(&updateWorldTimer, SIGNAL(timeout()), this, SLOT(updateWorld()));

    level=0;
}

GameField::~GameField()
{
    // Удаляются все динамические объекты, отображаемые на текущем уровне
    clearLevel();

    // Удаление обработчика пересечений
    delete contactListener;

    // Удаление движка физического моделирования
    delete physicsWorld;

    // Отключение физических объектов на стеке
    ball.setPhysicsWorld(nullptr);
    rocketBit.setPhysicsWorld(nullptr);

    // Отключение определителя положения ракетки
    moveDetector.doExit();
    moveDetectorThread.quit();
    moveDetectorThread.wait();
    while(!moveDetectorThread.isFinished()) {
        qDebug() << "Wait finished move detector...";
    }
    qDebug() << "Success move detector finished.";
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

    rocketBit.setPos(MOVE_NOID_START_BALL_POS_X, MOVE_NOID_START_BALL_POS_Y);
    ball.setPos(MOVE_NOID_START_ROCKET_BIT_POS_X, MOVE_NOID_START_ROCKET_BIT_POS_Y);
}


// Создание стандартных препятствий, ограничивающих экран игры
void GameField::initBarriers()
{
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

    // Временная балка снизу
    /*
    Barrier *barrierBottom=new Barrier();
    polygon.clear();
    polygon << QPointF(0.0, 0.0) << QPointF(0.0, 0.15) << QPointF(8.0, 0.15) << QPointF(8.0, 0.0);
    barrierBottom->setRotation(-10.0);
    barrierBottom->setPolygon(polygon);
    barrierBottom->setPos(1.5, 9.8);
    barriers.append( barrierBottom );
    this->addItem(barrierBottom);
    barrierBottom->setPhysicsWorld(physicsWorld);
    */
}


// Установки мячика
void GameField::initBall()
{
    ball.setRadius(0.15);
    ball.setPos(MOVE_NOID_START_BALL_POS_X, MOVE_NOID_START_BALL_POS_Y);
    this->addItem(&ball); // Мячик кладется на поле
    ball.setPhysicsWorld(physicsWorld);
}


// Установки ракетки
void GameField::initRocketBit()
{
    rocketBit.setPos(MOVE_NOID_START_ROCKET_BIT_POS_X, MOVE_NOID_START_ROCKET_BIT_POS_Y);
    this->addItem(&rocketBit); // Ракетка кладется на поле
    rocketBit.setPhysicsWorld(physicsWorld);
    rocketBit.setMoveDetector(&moveDetector);
}


// Загрузка уровня
void GameField::loadLevel(const int levelNum)
{
    clearLevel();
    initBarriers();

    if(levelNum==1){

        score=0;
        emit setScore(score);
        lives=5;
        emit setLives(lives);

        initBall();
        initRocketBit();

        // Создание кирпичей
        for(int i=0; i<5; i++) {
            qreal x=i*2+1.0;
            qreal y=4.0;
            createBrick(x,y);
        }
        for(int i=0; i<5; i++) {
            qreal x=i*2+1.5;
            qreal y=3.0;
            createBrick(x,y);
        }
        for(int i=0; i<5; i++) {
            qreal x=i*2+1.0;
            qreal y=2.0;
            createBrick(x,y);
        }
    }

    if(levelNum==2){
        for(int j=0; j<7; j++) {
            for(int i=0; i<12-j; i++) {
                qreal x=i*0.8+0.5;
                qreal y=4.5-j*0.5;
                createBrick(x,y);
            }
        }
    }

}


// Создание кирпича по игровым координатам
void GameField::createBrick(const qreal x, const qreal y)
{
    Brick *brick=new Brick;
    brick->setPos(x, y);
    bricks.append( brick ); // Запоминается указатель на кирпич
    this->addItem(brick); // Кирпич кладется на поле
    brick->setPhysicsWorld(physicsWorld);
}


// Метод удаляет кирпичи, с которыми столкнулся мячик
void GameField::destroyBricks()
{
    // Перебор кирпичей и удаление тех, с которым столкнулся мячик
    QList<Brick*>::iterator i = bricks.begin();
    while (i != bricks.end()) {
        Brick* brick=*i;

        if( brick->isToRemove() ) {
            // this->removeItem(brick); // Кирпич убирается с графического игрового поля
            delete brick; // Кирпич удаляется как объект
            i = bricks.erase(i); // Кирпич удаляется из списка кирпичей

            score+=10;
            emit setScore(score);
        }
        else
            ++i;
    }
}


void GameField::runGame()
{
    level=1;
    loadLevel(level);

    updateWorldTimer.start(1000/60);
}


// Обновление мира, этот слот срабатывает по таймеру updateWorldTimer
void GameField::updateWorld()
{
    // Обновляется физический мир
    physicsWorld->Step(1.0/60.0, 6, 2);

    // Удаляются кирпичи, помеченные на удаление в обрабочике пересечений
    destroyBricks();

    ball.updatePosByPhysicsWorld();       // Обновляется месторасположение мяча
    rocketBit.updatePosByMovieDetector(); // Обновляется месторасположение ракетки

    checkBallPosition(); // Проверка, не улетел ли мяч
    checkBricksCount();  // Проверка, не выбиты ли все кирпичи

    // Обновляется сцена
    this->update();
}


// Проверка местоположения мяча
void GameField::checkBallPosition()
{
    // Если мячь улетел
    if(ball.y()>10.0+1.0) {
        if(lives>0) { // Если еще есть попытки
            emit setLives(--lives);

            ReadyPoster readyPoster;
            readyPoster.setLine1Text("...READY...");
            readyPoster.exec();

            ball.moveToDefaultPos();
            rocketBit.moveToDefaultPos();
        } else {
            GameOverPoster gameOver;
            gameOver.setScore(score);
            gameOver.exec();

            emit closeGame();
        }
    }
}


// Проверка выбитых кирпичей
void GameField::checkBricksCount()
{
    // Если все кирпичи выбиты
    if(bricks.count()==0) {
        level++;

        ReadyPoster readyPoster;
        readyPoster.setLine1Text("Congratulation!");
        readyPoster.setLine2Text("Go to next level "+QString::number(level)+"!");
        readyPoster.exec();

        ball.moveToDefaultPos();
        rocketBit.moveToDefaultPos();
        loadLevel(level);
    }

}
