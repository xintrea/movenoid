#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include "Ball.h"
#include "Brick.h"

// Данный класс подключается к физическому движку
// для отслеживания событий контакта тел друг с другом

class ContactListener : public b2ContactListener
{
public:
    ContactListener();

    void setBall(Ball *iBall);
    void setBricks(QList<Brick*> *iBricks);

    void EndContact(b2Contact* contact);

protected:

    Ball *ball;
    QList<Brick*> *bricks; // В арканоидах имеет смысл отслеживать только события контакта мяча с кирпичами, чтобы кирпичи убирать

};

#endif // CONTACTLISTENER_H
