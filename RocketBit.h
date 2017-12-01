#ifndef ROCKETBIT_H
#define ROCKETBIT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

#include "opencv2/opencv.hpp"

#include "GraphicsPhysicsItem.h"
#include "MoveDetector.h"

class RocketBit : public GraphicsPhysicsItem
{
public:
    RocketBit(QGraphicsItem *parent = Q_NULLPTR);
    virtual ~RocketBit();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void putToPhysicsWorld();

    void updatePosByMovieDetector();
    void moveToDefaultPos();

    void setMoveDetector(MoveDetector *iMoveDetector);

protected:

    qreal width;
    qreal height;
    QColor color;

    MoveDetector *moveDetector;

    b2MouseJoint *mouseJoint;
};

#endif // ROCKETBIT_H
