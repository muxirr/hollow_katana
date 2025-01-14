#ifndef _SWOARD_H_
#define _SWOARD_H_

#include "animation.h"
#include "collision_manager.h"

class Sword
{
public:
    Sword(const Vector2 &pos, bool moveLeft);
    ~Sword();

    void update(float delta);
    void render();

    bool checkValid() const
    {
        return isValid;
    }

private:
    const float MOVE_SPEED = 1250.0f;

private:
    Vector2 position;
    Vector2 velocity;
    Animation animation;
    bool isValid = true;
    CollisionBox *collisionBox = nullptr;
};

#endif // _SWOARD_H_