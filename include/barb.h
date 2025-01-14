#ifndef _BARB_H_
#define _BARB_H_

#include "animation.h"
#include "collision_manager.h"

class Barb
{
public:
    Barb();
    ~Barb();

    void update(float delta);
    void render();

    void setPosition(const Vector2 &pos)
    {
        this->basePosition = pos;
        this->currentPosition = pos;
    }

    bool checkValid() const
    {
        return isValid;
    }

private:
    enum class Stage
    {
        Idle,
        Aim,
        Dash,
        Break
    };

private:
    const float MOVE_SPEED = 1500.0f;

private:
    Timer timerIdle;
    Timer timerAim;
    int diffPeriod = 0;
    bool isValid = true;
    float totalDeltaTime = 0.0f;

    Vector2 currentPosition;
    Vector2 basePosition;
    Vector2 velocity;

    Animation animationLoose;
    Animation animationBreak;
    Animation *currentAnimation = nullptr;

    Stage stage = Stage::Idle;
    CollisionBox *collisionBox = nullptr;

private:
    void breakBarb();
};

#endif // _BARB_H_
