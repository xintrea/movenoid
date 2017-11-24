#include "Ball.h"

Ball::Ball(QGraphicsItem *parent)
{
  QGraphicsItem::setParentItem(parent);

}

void Ball::setRadius(const qreal iRadius)
{
    radius=iRadius;
}

QRectF Ball::boundingRect() const
{
    return QRectF(-radius, -radius, radius*2, radius*2);
}


QPainterPath Ball::shape() const
{
    QPainterPath path;
    path.addEllipse(-radius, -radius, radius*2, radius*2);
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

    painter->drawEllipse(-radius, -radius, radius*2, radius*2);
}
