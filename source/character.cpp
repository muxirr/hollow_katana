#include "character.h"
#include "collision_manager.h"

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
    // 更新状态机
    stateMachine.update(delta);

    if (hp <= 0)
    {
        velocity.x = 0;
    }

    // 更新位置
    if (enableGravity)
    {
        velocity.y += GRAVITY * delta;
    }

    position += velocity * delta;

    // 角色落地
    if (position.y > FLOOR_Y)
    {
        position.y = FLOOR_Y;
        velocity.y = 0;
    }

    // 限制角色在屏幕内
    if (position.x < 0)
    {
        position.x = 0;
    }
    if (position.x > getwidth())
    {
        position.x = (float)getwidth();
    }

    // 更新碰撞箱
    hurtBox->setPosition(getLogicCenter());

    // 更新无敌状态
    timerInvulnerableStatus.update(delta);

    if (isInvulnerable)
    {
        timerInvulnerableBlink.update(delta);
    }

    // 更新动画
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
    // do nothing
}

void Character::input(const ExMessage &msg)
{
    // do nothing
}

void Character::hurted()
{
    // do nothing
}
