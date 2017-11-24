#include "Brick.h"

Brick::Brick(QGraphicsItem *parent)
{
    QGraphicsItem::setParentItem(parent);
}

QRectF Brick::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
}


QPainterPath Brick::shape() const
{
    QPainterPath path;
    path.addRect(-0.025, -0.025, 0.05, 0.05);
    return path;
}



void Brick::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    painter->setBrush(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
    painter->drawEllipse(-0.025, -0.025, 0.05, 0.05);
}
