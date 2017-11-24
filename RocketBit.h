#ifndef ROCKETBIT_H
#define ROCKETBIT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>


class RocketBit : public QGraphicsItem
{
public:
    RocketBit(QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

};

#endif // ROCKETBIT_H
