#include "Brick.h"

Brick::Brick(QGraphicsItem *parent)
{
    QGraphicsItem::setParentItem(parent);

    width=0.8;
    height=0.4;

    // color=QColor(qrand() % 256, qrand() % 256, qrand() % 256);
    color=QColor(156, 44, 44);

    flagToRemove=false;
}

QRectF Brick::boundingRect() const
{
    qreal halhWidth=width/2.0;
    qreal halhHeight=height/2.0;
    return QRectF(-halhWidth, -halhHeight, width, height);
}


QPainterPath Brick::shape() const
{
    QPainterPath path;
    path.addRect( boundingRect() );
    return path;
}


void Brick::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Заливка
    painter->setBrush(color);

    // Граница
    QPen pen;
    pen.setWidth(0.1);
    pen.setBrush(Qt::white);
    painter->setPen(pen);

    painter->drawRect( boundingRect() );
}


void Brick::putToPhysicsWorld()
{
    b2BodyDef bodyDef;
    bodyDef.type=b2_staticBody;
    bodyDef.position.Set(this->x(), this->y());
    b2Body *body=physicsWorld->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(width/2.0, height/2.0);

    body->CreateFixture(&polygonShape, 0.0);

    // Запоминается настроенное тело
    physicsBody=body;
}


void Brick::setSize(const qreal iWidth, const qreal iHeight)
{
    width=iWidth;
    height=iHeight;
}


void Brick::setToRemove()
{
    flagToRemove=true; // Кирпич подготавливается к уничтожению с игрового поля
}


bool Brick::isToRemove() const
{
    return flagToRemove;
}

