#include <QDebug>
#include "GraphicsPhysicsItem.h"

GraphicsPhysicsItem::GraphicsPhysicsItem(QGraphicsItem *parent)
{
    QGraphicsItem::setParentItem(parent);

    physicsWorld=nullptr;
    physicsBody=nullptr;
}


GraphicsPhysicsItem::~GraphicsPhysicsItem()
{
    // Элемент убирается из физического симулятора
    if(physicsBody!=nullptr)
        physicsWorld->DestroyBody(physicsBody);
}


void GraphicsPhysicsItem::setPhysicsWorld(b2World *iPhysicsWorld)
{
    physicsWorld=iPhysicsWorld;

    putToPhysicsWorld();
}


void GraphicsPhysicsItem::putToPhysicsWorld()
{
    qDebug() << "It is empty method putToPhysicsWorld()";
}


b2Body *GraphicsPhysicsItem::getPhysicsBody() const
{
    return physicsBody;
}
