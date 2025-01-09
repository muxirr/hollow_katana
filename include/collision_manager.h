#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

#include "collision_box.h"

#include <vector>

class CollisionManager
{
public:
    static CollisionManager *Instance();

    CollisionBox *createCollisionBox();
    void destroyCollisionBox(CollisionBox *collisionBox);

    void processCollide();
    void onDebugRender();

private:
    static CollisionManager *manager;

    std::vector<CollisionBox *> collisionBoxList;

private:
    CollisionManager();
    ~CollisionManager();
};

#endif // _COLLISION_MANAGER_H_