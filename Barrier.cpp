#include <QDebug>
#include "Barrier.h"
#include "main.h"

Barrier::Barrier(QGraphicsItem *parent)
{
    QGraphicsItem::setParentItem(parent);
}


void Barrier::setPolygon(const QPolygonF &iPolygon)
{
    polygon=iPolygon;
}

void Barrier::putToPhysicsWorld()
{

}


QRectF Barrier::boundingRect() const
{
    return polygon.boundingRect();
}


QPainterPath Barrier::shape() const
{
    QPainterPath path;
    path.addPolygon( polygon );
    return path;
}



void Barrier::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Filling
    painter->setBrush(QColor(87, 113, 140));

    // Edges
    QPen pen;
    pen.setWidth(0.1);
    pen.setBrush(Qt::white);
    painter->setPen(pen);

    painter->drawPolygon( polygon );
}
