#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "vector2.h"
#include "animation.h"
#include "collision_box.h"
#include "state_machine.h"

#include <string>
#include <graphics.h>
#include <unordered_map>

class Character
{
public:
    Character();
    ~Character();

    void decreaseHp();

    int getHp() const
    {
        return hp;
    }

    void setPosition(const Vector2 &position)
    {
        this->position = position;
    }

    const Vector2 &getPosition() const
    {
        return position;
    }

    void setVelocity(const Vector2 &velocity)
    {
        this->velocity = velocity;
    }

    const Vector2 &getVelocity() const
    {
        return velocity;
    }

    Vector2 getLogicCenter() const
    {
        return Vector2(position.x, position.y - logicHeight / 2);
    }

    void setGravityEnable(bool flag)
    {
        this->enableGravity = flag;
    }

    CollisionBox *getHitBox() const
    {
        return hitBox;
    }

    CollisionBox *getHurtBox() const
    {
        return hurtBox;
    }

    bool isOnFloor() const
    {
        return position.y >= FLOOR_Y;
    }

    float getFloorY() const
    {
        return FLOOR_Y;
    }

    void makeInvulnerable()
    {
        isInvulnerable = true;
        timerInvulnerableStatus.restart();
    }

    virtual void input(const ExMessage &msg);
    virtual void update(float delta);
    virtual void render();

    virtual void hurted();

    void switchState(const std::string &id)
    {
        stateMachine.switchTo(id);
    }

    void setAnimation(const std::string &id)
    {
        currentAnimation = &animationPool[id];

        currentAnimation->left.reset();
        currentAnimation->right.reset();
    }

    virtual void log() = 0;

protected:
    struct AnimationGroup
    {
        Animation left;
        Animation right;
    };

protected:
    // 地板Y坐标1
    const float FLOOR_Y = 620;

    // 重力1
    const float GRAVITY = 980 * 2.0;

protected:
    // 角色生命值1
    int hp = 10;

    // 角色脚底位置1
    Vector2 position;

    // 角色速度1
    Vector2 velocity;

    // 角色逻辑高度1
    float logicHeight = 0;

    // 当前角色是否朝向左1
    bool isFacingLeft = true;

    // 角色逻辑状态机1
    StateMachine stateMachine;

    // 启用重力模拟1
    bool enableGravity = true;

    // 当前是否无敌1
    bool isInvulnerable = false;

    // 无敌闪烁状态定时器1
    Timer timerInvulnerableBlink;

    // 无敌状态定时器1
    Timer timerInvulnerableStatus;

    // 当前是否处于闪烁的不可见帧1
    bool isBlinkInvisible = false;

    // 攻击碰撞箱1
    CollisionBox *hitBox = nullptr;

    // 受击碰撞箱1
    CollisionBox *hurtBox = nullptr;

    // 当前角色动画1
    AnimationGroup *currentAnimation = nullptr;

    // 角色动画池1
    std::unordered_map<std::string, AnimationGroup> animationPool;
};

#endif // _CHARACTER_H_