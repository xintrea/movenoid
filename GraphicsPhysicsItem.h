#ifndef GRAPHICSPHYSICSITEM_H
#define GRAPHICSPHYSICSITEM_H

#include <QGraphicsItem>
#include <Box2D/Box2D.h>

class GraphicsPhysicsItem : public QGraphicsItem
{
public:
    GraphicsPhysicsItem(QGraphicsItem *parent = Q_NULLPTR);
    virtual ~GraphicsPhysicsItem();

    void setPhysicsWorld(b2World *iPhysicsWorld);
    virtual void putToPhysicsWorld();

protected:
    b2World *physicsWorld;
    b2Body *physicsBody;

};

#endif // GRAPHICSPHYSICSITEM_H
