#ifndef BRICK_H
#define BRICK_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

#include <GraphicsPhysicsItem.h>

class Brick : public GraphicsPhysicsItem
{
public:
    Brick(QGraphicsItem *parent = Q_NULLPTR);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void putToPhysicsWorld();

    void setSize(const qreal iWidth, const qreal iHeight);
    void setToRemove();
    bool isToRemove() const;

protected:
    qreal width;
    qreal height;

    QColor color;

    bool flagToRemove;
};

#endif // BRICK_H
