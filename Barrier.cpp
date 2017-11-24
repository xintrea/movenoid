#include <QDebug>
#include "Barrier.h"

Barrier::Barrier(QGraphicsItem *parent)
{
    QGraphicsPolygonItem::setParentItem(parent);
}


Barrier::Barrier(const QPolygonF &polygon, QGraphicsItem *parent)
{
    QGraphicsPolygonItem::setParentItem(parent);
    QGraphicsPolygonItem::setPolygon(polygon);
}


QRectF Barrier::boundingRect() const
{
    qreal minByX=10000.0;
    qreal maxByX=-100000.0;
    qreal minByY=10000.0;
    qreal maxByY=-100000.0;

    foreach (QPointF point, this->polygon() ) {
        qDebug() << "Polygon Coord: " << point.x() << point.y();

        if(point.x()<minByX) minByX=point.x();
        if(point.x()>maxByX) maxByX=point.x();

        if(point.y()<minByY) minByY=point.y();
        if(point.y()>maxByY) maxByY=point.y();
    }

    qDebug() << "Bounding Rect: " << minByX << minByY << maxByX << maxByY;

    return QRectF( QPointF(minByX, minByY), QPointF(maxByX, maxByY) );
}


QPainterPath Barrier::shape() const
{
    QPainterPath path;
    path.addPolygon( this->polygon() );
    return path;
}



void Barrier::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    painter->setBrush(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
    painter->drawPolygon( this->polygon() );
}
