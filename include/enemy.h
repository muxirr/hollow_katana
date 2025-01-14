#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "barb.h"
#include "sword.h"
#include "character.h"

class Enemy : public Character
{
public:
    Enemy();
    ~Enemy();

    void update(float delta) override;
    void render() override;

    void hurted();

    void setFacingLeft(bool isFacingLeft)
    {
        this->isFacingLeft = isFacingLeft;
    }

    bool getFacingLeft() const
    {
        return isFacingLeft;
    }

    void setDashInAir(bool isDashInAir)
    {
        this->isDashInAir = isDashInAir;
    }

    bool getDashInAir() const
    {
        return isDashInAir;
    }

    void setDashOnFloor(bool isDashOnFloor)
    {
        this->isDashOnFloor = isDashOnFloor;
    }

    bool getDashOnFloor() const
    {
        return isDashOnFloor;
    }

    void setThrowingSlik(bool isThrowingSlik)
    {
        this->isThrowingSlik = isThrowingSlik;
    }

    bool getThrowingSlik() const
    {
        return isThrowingSlik;
    }

    void throwBarbs();
    void throwSword();
    void throwSlik();

    void dash();

private:
    bool isThrowingSlik = false;
    bool isDashInAir = false;
    bool isDashOnFloor = false;

    Timer timerLog;

    Animation animationSilk;
    AnimationGroup animationDashInAirVfx;
    AnimationGroup animationDashOnFloorVfx;
    Animation *currentDashAnimation = nullptr;

    std::vector<Barb *> barbList;
    std::vector<Sword *> swordList;
    CollisionBox *collisionBoxSilk = nullptr;
};

#endif // _ENEMY_H_
