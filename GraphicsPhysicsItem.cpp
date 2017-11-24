#include "GraphicsPhysicsItem.h"

GraphicsPhysicsItem::GraphicsPhysicsItem(QGraphicsItem *parent)
{
    QGraphicsItem::setParentItem(parent);
}

void GraphicsPhysicsItem::setPhysicsWorld(b2World *iPhysicsWorld)
{
    physicsWorld=iPhysicsWorld;
}
