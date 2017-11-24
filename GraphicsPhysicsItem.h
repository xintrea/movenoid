#ifndef GRAPHICSPHYSICSITEM_H
#define GRAPHICSPHYSICSITEM_H

#include <QGraphicsItem>
#include <Box2D/Box2D.h>

class GraphicsPhysicsItem : public QGraphicsItem
{
public:
    GraphicsPhysicsItem(QGraphicsItem *parent = Q_NULLPTR);

    void setPhysicsWorld(b2World *iPhysicsWorld);
    virtual void putToPhysicsWord();

protected:
    b2World *physicsWorld;

};

#endif // GRAPHICSPHYSICSITEM_H
