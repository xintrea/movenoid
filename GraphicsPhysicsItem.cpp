#include "GraphicsPhysicsItem.h"

GraphicsPhysicsItem::GraphicsPhysicsItem(QGraphicsItem *parent)
{
    QGraphicsItem::setParentItem(parent);

    physicsWorld=nullptr;
}


GraphicsPhysicsItem::~GraphicsPhysicsItem()
{

}


void GraphicsPhysicsItem::setPhysicsWorld(b2World *iPhysicsWorld)
{
    physicsWorld=iPhysicsWorld;
}


void GraphicsPhysicsItem::putToPhysicsWorld()
{

}
