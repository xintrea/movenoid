#include <QDebug>
#include "Barrier.h"
#include "main.h"

Barrier::Barrier(QGraphicsItem *parent)
{
    QGraphicsItem::setParentItem(parent);
}


void Barrier::setPolygon(const QPolygonF &iPolygon)
{
    polygon=iPolygon;
}


void Barrier::putToPhysicsWorld()
{
    b2BodyDef bodyDef;
    bodyDef.type=b2_staticBody;
    // bodyDef.position.Set(this->x(), this->y());
    bodyDef.position.Set(0.0, 0.0);
    b2Body *body=physicsWorld->CreateBody(&bodyDef);

    // b2PolygonDef shapeDef;

    QPolygonF scenePolygon=mapToScene(polygon);

    qDebug() << "New polygon";

    b2Vec2 vertices[10];
    for(int i=0; i<scenePolygon.size(); i++) { // // for(int i=scenePolygon.size()-1; i>=0; i--)
        vertices[i].Set(scenePolygon.at(i).x(), scenePolygon.at(i).y());

        qDebug() << "Item coordinats:" << polygon.at(i).x() << polygon.at(i).y() << "Scene coordinats:" << scenePolygon.at(i).x() << scenePolygon.at(i).y() ;
    }
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, scenePolygon.size());

    // body->CreateShape(&polygonShape);
    body->CreateFixture(&polygonShape, 0.0);

    // Запоминается настроенное тело
    physicsBody=body;
}


QRectF Barrier::boundingRect() const
{
    return polygon.boundingRect();
}


QPainterPath Barrier::shape() const
{
    QPainterPath path;
    path.addPolygon( polygon );
    return path;
}



void Barrier::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Filling
    painter->setBrush(QColor(87, 113, 140));

    // Edges
    QPen pen;
    pen.setWidth(0.1);
    pen.setBrush(Qt::white);
    painter->setPen(pen);

    painter->drawPolygon( polygon );
}
