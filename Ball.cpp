#include <QDebug>
#include <Box2D/Box2D.h>
#include "Ball.h"

Ball::Ball(QGraphicsItem *parent)
{
  QGraphicsItem::setParentItem(parent);
}


void Ball::setRadius(const qreal iRadius)
{
    radius=iRadius;
}


void Ball::putToPhysicsWorld()
{
    b2BodyDef bodyDef;
    bodyDef.type=b2_dynamicBody;

    b2Body *body=physicsWorld->CreateBody(&bodyDef);
    b2CircleShape shape;
    shape.m_radius=radius;

    body->CreateFixture(&shape, 1.0);
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
