#ifndef BALL_H
#define BALL_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class Ball : public QGraphicsItem
{
public:
    Ball(QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setRadius(const qreal iRadius);

protected:

    qreal radius;

};

#endif // BALL_H
