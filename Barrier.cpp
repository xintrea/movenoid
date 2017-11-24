#include <QDebug>
#include "Barrier.h"
#include "main.h"

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
    static bool isInit=false;

    static qreal cmp=10000.0;
    static qreal minByX=cmp;
    static qreal maxByX=-cmp;
    static qreal minByY=cmp;
    static qreal maxByY=-cmp;

    if(!isInit) {
        foreach (QPointF point, this->polygon() ) {
            qDebug() << "Polygon Coord: " << point.x() << point.y();

            if(point.x()<minByX) minByX=point.x();
            if(point.x()>maxByX) maxByX=point.x();

            if(point.y()<minByY) minByY=point.y();
            if(point.y()>maxByY) maxByY=point.y();
        }

        if(minByX==cmp || maxByX==-cmp || minByY==cmp || maxByY==-cmp)
            criticalError("Bad barrier polygon: "+QString::number(minByX)+" "+QString::number(minByY)+" "+QString::number(maxByX)+" "+QString::number(maxByY));

        isInit=true;

        qDebug() << "Bounding Rect: " << minByX << minByY << maxByX << maxByY;
    }

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
    // Filling
    painter->setBrush(QColor(87, 113, 140));

    // Edges
    QPen pen;
    pen.setWidth(0.1);
    pen.setBrush(Qt::white);
    painter->setPen(pen);

    painter->drawPolygon( this->polygon() );
}
