#include <QDebug>
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
    qDebug() << "Call ball boundingRect, radius:" << radius << " size: " << radius*2.0;
    return QRectF(-radius, -radius, radius*2.0, radius*2.0);
}


QPainterPath Ball::shape() const
{
    qDebug() << "Call ball shape, radius:" << radius << " size: " << radius*2.0;

    QPainterPath path;
    path.addEllipse(-radius, -radius, radius*2.0, radius*2.0);
    // path.addRect(-radius, -radius, radius*2.0, radius*2.0);
    return path;
}


void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    qDebug() << "Call ball paint, radius:" << radius << " size: " << radius*2.0;

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
