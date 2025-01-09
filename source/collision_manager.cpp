#include "collision_manager.h"

#include <graphics.h>

CollisionBox *CollisionManager::createCollisionBox()
{
    CollisionBox *collisionBox = new CollisionBox();
    collisionBoxList.push_back(collisionBox);
    return collisionBox;
}

void CollisionManager::destroyCollisionBox(CollisionBox *collisionBox)
{
    collisionBoxList.erase(std::remove(collisionBoxList.begin(), collisionBoxList.end(), collisionBox), collisionBoxList.end());

    delete collisionBox;
}

void CollisionManager::processCollide()
{
    for (CollisionBox *src : collisionBoxList)
    {
        if (!src->enabled || src->layerDst == CollisionLayer::None)
        {
            continue;
        }
        for (CollisionBox *dst : collisionBoxList)
        {
            if (src == dst || !dst->enabled || dst->layerSrc != src->layerDst)
            {
                continue;
            }

            bool isCollide = (bool)(max(src->position.x + src->size.x / 2, dst->position.x + dst->size.x / 2) - min(src->position.x - src->size.x / 2, dst->position.x - dst->size.x / 2)) && (bool)(max(src->position.y + src->size.y / 2, dst->position.y + dst->size.y / 2) - min(src->position.y - src->size.y / 2, dst->position.y - dst->size.y / 2));
            if (isCollide && src->onCollide)
            {
                src->onCollide();
            }
        }
    }
}

void CollisionManager::onDebugRender()
{
    for (CollisionBox *collisionBox : collisionBoxList)
    {
        setlinecolor(collisionBox->enabled ? RGB(255, 195, 195) : RGB(115, 115, 175));
        rectangle(collisionBox->position.x - collisionBox->size.x / 2, collisionBox->position.y - collisionBox->size.y / 2, collisionBox->position.x + collisionBox->size.x / 2, collisionBox->position.y + collisionBox->size.y / 2);
    }
}

CollisionManager *CollisionManager::manager = nullptr;

CollisionManager *CollisionManager::Instance()
{
    if (!manager)
    {
        manager = new CollisionManager();
    }
    return manager;
}

CollisionManager::CollisionManager() = default;
CollisionManager::~CollisionManager() = default;