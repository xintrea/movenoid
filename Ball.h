#ifndef BALL_H
#define BALL_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include "GraphicsPhysicsItem.h"

class Ball : public GraphicsPhysicsItem
{
public:
    Ball(QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setRadius(const qreal iRadius);
    void putToPhysicsWorld();
    void updatePosByPhysicsWorld();

protected:

    qreal radius;
};

#endif // BALL_H
