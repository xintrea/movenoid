#include <QDebug>

#include "main.h"
#include "RocketBit.h"

RocketBit::RocketBit(QGraphicsItem *parent)
{
    QGraphicsItem::setParentItem(parent);

    width=1.1;
    height=0.45;
    color=QColor(31, 56, 94);

    moveDetector=nullptr;
    mouseJoint=nullptr;
}


RocketBit::~RocketBit()
{

}


void RocketBit::setMoveDetector(MoveDetector *iMoveDetector)
{
    moveDetector=iMoveDetector;
}


QRectF RocketBit::boundingRect() const
{
    qreal halhWidth=width/2.0;
    qreal halhHeight=height/2.0;
    return QRectF(-halhWidth, -halhHeight, width, height);
}


QPainterPath RocketBit::shape() const
{
    QPainterPath path;
    path.addRect( boundingRect() );
    return path;
}


void RocketBit::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen; // Обводка

    // Корпус ракетки
    painter->setBrush(color); // Заливка
    pen.setWidth(0.1);
    pen.setBrush(Qt::white);
    painter->setPen(pen);
    painter->drawRect( boundingRect() );


    // Огонь из левой дюзы
    painter->setBrush(QColor(150+(qrand()%(254-150)), 50, 50)); // Заливка
    pen.setWidth(0.2);
    pen.setBrush(Qt::yellow);

    QPolygonF polygon;
    qreal flameWidth=0.2;
    qreal flameHeight=0.25;
    polygon << QPointF(-width/2.0*0.8, height/2+0.1)
            << QPointF(-width/2.0*0.8-flameWidth/2.0, height/2+0.1+flameHeight)
            << QPointF(-width/2.0*0.8+flameWidth/2.0, height/2+0.1+flameHeight);
    painter->drawPolygon( polygon );

    // Огонь из правой дюзы
    polygon.translate(width/2.0*0.8 * 2, 0.0);
    painter->drawPolygon( polygon );
}


// Ракетка при каждом обращении удаляется и снова создается в физическом мире
void RocketBit::putToPhysicsWorld()
{
    static qreal previousX=5.0;
    static qreal previousY=5.0;

    // Если ракетка уже была проинициализирована, она удаляется (чтобы произошло пересоздание)
    if(physicsBody!=nullptr) {
        physicsWorld->DestroyBody(physicsBody);
    }

    // Вычисляется вектор движения ракетки, чтобы преобразовать его в вектор скорости
    b2Vec2 velocityVector(this->x()-previousX, this->y()-previousY);
    previousX=this->x();
    previousY=this->y();

    // Ракетка создается
    b2BodyDef bodyDef;
    bodyDef.type=b2_staticBody; // b2_dynamicBody
    bodyDef.position.Set(this->x(), this->y());
    bodyDef.angle=degToRad(this->rotation());
    b2Body *body=physicsWorld->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(width/2.0, height/2.0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 1.0; // Плотность
    fixtureDef.restitution=0.99; // Коэфициент отскока?

    body->CreateFixture(&fixtureDef);
    body->SetLinearVelocity(velocityVector);

    // Запоминается настроенное тело
    physicsBody=body;
}


/*
void RocketBit::putToPhysicsWorld()
{
    // Создается тело ракетки
    b2BodyDef bodyDef;
    bodyDef.type=b2_dynamicBody;
    bodyDef.position.Set(this->x(), this->y());
    bodyDef.angle=degToRad(this->rotation());
    b2Body *body=physicsWorld->CreateBody(&bodyDef);



    b2MouseJointDef mouseJointDef;

    // Получение ground body в C++ в box2d 2.x.x, чтобы прикрепиться к миру
    b2BodyDef groundBodyDef;
    b2Body groundBody = CreateBody(&groundBodyDef);

    mouseJointDef.bodyA=groundBody;
    }

}
*/

void RocketBit::updatePosByMovieDetector()
{
    // moveDetector.update(); // Теперь этот вызов ненужен, потому что moveDetector работает в треде

    // В дальнейших вычислениях принимается, что MoveDetector выдает координаты в виде игровых координат x=0...10, y=0...10
    // пропорционально растягивая прямоугольную картинку камеры до квадрата

    // Местоположение оси в игре, на которой должна отображаться горизонтальная центральная ось картинки с камеры
    qreal yOpticAxis=7.5; // В координатах сцены

    qreal yKoeff=1.5; // Коэффициент сплющивания координат по вертикали

    qreal yShift=10.0-(10.0*yKoeff)/2.0; // Зазор от верхнего края экрана до сплющенного пространства
    yShift=yShift+(10.0-yOpticAxis); // Зазор увеличиватся в соответствии с местоположением оси картинки с камеры

    // Местоположение ракетки
    QPointF pos=moveDetector->getRocketBitPos();

    // Координаты, полученные с камеры, сплющиваются по вертикали и сдвигаются до оси в игре
    qreal yPos=pos.y()/yKoeff+yShift;

    if(yPos<5.0)
        yPos=5.0;
    this->setPos( QPointF(pos.x(), yPos) );

    // Поворот ракетки
    this->setRotation( moveDetector->getRocketBitAngle() );

    putToPhysicsWorld(); // Чтобы ракетка пересоздавалась в новом месте


    // qDebug() << "RocketBit coordinats 1: " << moveDetector.getRocketBitPos();
    // qDebug() << "RocketBit coordinats 2: " << this->x() << this->y();
}
