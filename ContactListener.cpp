#include "ContactListener.h"


ContactListener::ContactListener()
{
    ball=nullptr;
    bricks=nullptr;
}

void ContactListener::setBall(Ball *iBall)
{
    ball=iBall;
}

void ContactListener::setBricks(QList<Brick *> *iBricks)
{
    bricks=iBricks;
}


// Метод обработки события когда два тела перестают пересекаться
// Отскок мяча должен происходить только после того, как контакт пропал
void ContactListener::EndContact(b2Contact* contact)
{
    b2Body *bodyA=contact->GetFixtureA()->GetBody();
    b2Body *bodyB=contact->GetFixtureB()->GetBody();

    // Нахождение кирпича, с которым перестал пересекаться мячик
    foreach( Brick* brick, *bricks )
        if( bodyA==brick->getPhysicsBody() || bodyB==brick->getPhysicsBody())
            brick->setToRemove(); // Кирпич помачается на удаление. Сразу здесь в обработчике удалять нельзя
}
