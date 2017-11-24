#ifndef BARRIER_H
#define BARRIER_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPolygonF>

#include "GraphicsPhysicsItem.h"

class Barrier : public GraphicsPhysicsItem
{
public:
    Barrier(QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setPolygon(const QPolygonF &iPolygon);

protected:

    QPolygonF polygon;

};

#endif // BARRIER_H
