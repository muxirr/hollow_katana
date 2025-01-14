#include "sword.h"
#include "resources_manager.h"
#include "collision_manager.h"

Sword::Sword(const Vector2 &pos, bool moveLeft)
{
    animation.setInterval(0.1f);
    animation.setLoop(true);
    animation.setAnchorMode(Animation::AnchorMode::Centered);
    animation.addFrame(*ResourcesManager::Instance()->findAtlas(moveLeft ? ("swordLeft") : _T("swordRight")));

    collisionBox = CollisionManager::Instance()->createCollisionBox();
    collisionBox->setLayerSrc(CollisionLayer::None);
    collisionBox->setLayerDst(CollisionLayer::Player);
    collisionBox->setSize({195, 10});

    this->position = pos;
    this->velocity = {moveLeft ? -MOVE_SPEED : MOVE_SPEED, 0};
}

Sword::~Sword()
{
    CollisionManager::Instance()->destroyCollisionBox(collisionBox);
}

void Sword::update(float delta)
{
    position += velocity * delta;
    animation.setPosition(position);
    collisionBox->setPosition(position);

    animation.update(delta);

    if (position.x <= -200 || position.x >= getwidth() + 200)
    {
        isValid = false;
    }
}

void Sword::render()
{
    animation.render();
}