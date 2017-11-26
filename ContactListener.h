#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include "Ball.h"
#include "Brick.h"

class ContactListener : public b2ContactListener
{
public:
    ContactListener();

    void setBall(Ball *iBall);
    void setBricks(QList<Brick*> *iBricks);

    void EndContact(b2Contact* contact);

protected:

    Ball *ball;
    QList<Brick*> *bricks;

};

#endif // CONTACTLISTENER_H
