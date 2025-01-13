#include "character.h"
#include "collision_manager.h"

#include <string>
#include <iostream>

Character::Character()
{
    hitBox = CollisionManager::Instance()->createCollisionBox();
    hurtBox = CollisionManager::Instance()->createCollisionBox();

    timerInvulnerableStatus.setWaitTime(1.0f);
    timerInvulnerableStatus.setOneShot(true);
    timerInvulnerableStatus.setTimeOut([&]()
                                       { isInvulnerable = false; });

    timerInvulnerableBlink.setWaitTime(0.075f);
    timerInvulnerableBlink.setOneShot(false);
    timerInvulnerableBlink.setTimeOut([&]()
                                      { isBlinkInvisible = !isBlinkInvisible; });
}

Character::~Character()
{
    CollisionManager::Instance()->destroyCollisionBox(hitBox);
    CollisionManager::Instance()->destroyCollisionBox(hurtBox);
}

void Character::decreaseHp()
{
    if (isInvulnerable)
    {
        return;
    }

    hp -= 1;
    if (hp > 0)
    {
        makeInvulnerable();
    }
    hurted();
}

void Character::update(float delta)
{
    // 更新状态机1
    stateMachine.update(delta);

    if (hp <= 0)
    {
        velocity.x = 0;
    }

    // 更新位置1
    if (enableGravity)
    {
        velocity.y += GRAVITY * delta;
    }

    position += velocity * delta;

    // 角色落地1
    if (position.y > FLOOR_Y)
    {
        position.y = FLOOR_Y;
        velocity.y = 0;
    }

    // 限制角色在屏幕内1
    if (position.x < 0)
    {
        position.x = 0;
    }
    if (position.x > getwidth())
    {
        position.x = (float)getwidth();
    }

    // 更新碰撞箱1
    hurtBox->setPosition(getLogicCenter());

    // 更新无敌状态1
    timerInvulnerableStatus.update(delta);

    if (isInvulnerable)
    {
        timerInvulnerableBlink.update(delta);
    }

    // 更新动画1
    if (!currentAnimation)
    {
        return;
    }

    Animation &animation = isFacingLeft ? currentAnimation->left : currentAnimation->right;
    animation.update(delta);
    animation.setPosition(position);
}

void Character::render()
{
    if (!currentAnimation || (isInvulnerable && isBlinkInvisible))
    {
        return;
    }
    (isFacingLeft ? currentAnimation->left : currentAnimation->right).render();
}

void Character::input(const ExMessage &msg)
{
    // do nothing
}

void Character::hurted()
{
    // do nothing
}