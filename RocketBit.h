#ifndef ROCKETBIT_H
#define ROCKETBIT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

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

    void setMoveDetector(MoveDetector *iMoveDetector);

protected:

    qreal width;
    qreal height;
    QColor color;

    MoveDetector *moveDetector;
};

#endif // ROCKETBIT_H
