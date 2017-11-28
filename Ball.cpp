#include <QDebug>
#include <Box2D/Box2D.h>
#include "Ball.h"

Ball::Ball(QGraphicsItem *parent)
{
  QGraphicsItem::setParentItem(parent);

  radius=0.0;
}


void Ball::setRadius(const qreal iRadius)
{
    radius=iRadius;
}


void Ball::putToPhysicsWorld()
{
    // Общие параметры тела
    b2BodyDef bodyDef;
    bodyDef.type=b2_dynamicBody;
    bodyDef.position.Set(this->x(), this->y());

    // Создается тело в физическом движке
    b2Body *body=physicsWorld->CreateBody(&bodyDef);

    // Создается форма тела
    b2CircleShape circleShape;
    circleShape.m_radius=radius;

    // Настройка физических параметров тела
    b2FixtureDef fixture;
    fixture.shape = &circleShape; // Форма
    fixture.density = 1.0;        // Плотность (через нее и форму движек узнает массу тела)
    fixture.friction = 0.0;       // Коэффициент трения ( 0.0 - нет трения, 1.0 - максимальное трение. Трение расчитывается "среднее" для двух контачащих тел)
    fixture.restitution = 1.0;    // Коэффициент упругости (0.0 - нет отскока, 1.0 - максимальный отскок)
    body->CreateFixture(&fixture);

    qDebug() << "Ball mass: " << body->GetMass();

    // Запоминается настроенное тело
    physicsBody=body;
}


void Ball::updatePosByPhysicsWorld()
{
    this->setX( physicsBody->GetPosition().x );
    this->setY( physicsBody->GetPosition().y );

    // qDebug() << "Ball coordinats: " << this->x() << this->y();
}


b2ContactEdge *Ball::getContactList()
{
    return physicsBody->GetContactList();
}


QRectF Ball::boundingRect() const
{
    return QRectF(-radius, -radius, radius*2.0, radius*2.0);
}


QPainterPath Ball::shape() const
{
    QPainterPath path;
    path.addEllipse(-radius, -radius, radius*2.0, radius*2.0);
    return path;
}


void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Filling
    painter->setBrush(QColor(199, 104, 2));

    // Edges
    QPen pen;
    pen.setWidth(0.1);
    pen.setBrush(Qt::white);
    painter->setPen(pen);

    QRectF rectangle(-radius, -radius, radius*2.0, radius*2.0);
    painter->drawEllipse(rectangle);
}
