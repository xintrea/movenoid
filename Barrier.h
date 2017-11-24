#ifndef BARRIER_H
#define BARRIER_H

#include <QGraphicsPolygonItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class Barrier : public QGraphicsPolygonItem
{
public:
    Barrier(QGraphicsItem *parent = Q_NULLPTR);
    Barrier(const QPolygonF &polygon, QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
protected:


};

#endif // BARRIER_H
