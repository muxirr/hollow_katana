#include "barb.h"
#include "collision_manager.h"
#include "resources_manager.h"
#include "character_manager.h"

Barb::Barb()
{
    diffPeriod = rangeRandom(0, 6);
    animationLoose.setInterval(0.15f);
    animationLoose.setLoop(true);
    animationLoose.setAnchorMode(Animation::AnchorMode::Centered);
    animationLoose.addFrame(*ResourcesManager::Instance()->findAtlas(_T("barbLoose")));

    animationBreak.setInterval(0.1f);
    animationBreak.setLoop(false);
    animationBreak.setAnchorMode(Animation::AnchorMode::Centered);
    animationBreak.addFrame(*ResourcesManager::Instance()->findAtlas(_T("barbBreak")));
    animationBreak.setOnFinished([&]()
                                 { isValid = false; });

    collisionBox = CollisionManager::Instance()->createCollisionBox();
    collisionBox->setLayerSrc(CollisionLayer::Enemy);
    collisionBox->setLayerDst(CollisionLayer::Player);
    collisionBox->setSize({20, 20});
    collisionBox->setOnCollide([&]()
                               { breakBarb(); });

    timerIdle.setWaitTime((float)rangeRandom(3, 10));
    timerIdle.setOneShot(true);
    timerIdle.setTimeOut([&]()
                         {if(stage == Stage::Idle){
                            stage = Stage::Aim;
                            basePosition = currentPosition;
        } });

    timerAim.setWaitTime(0.75f);
    timerAim.setOneShot(true);
    timerAim.setTimeOut([&]()
                        {if(stage == Stage::Aim){
                            stage = Stage::Dash;
                            const Vector2 &playerPos = CharacterManager::Instance()->getPlayer()->getPosition();
                            velocity = (playerPos - currentPosition).normalize() * MOVE_SPEED;
        } });
}

Barb::~Barb()
{
    CollisionManager::Instance()->destroyCollisionBox(collisionBox);
}

void Barb::update(float delta)
{
    // 更新定时器1
    if (stage == Stage::Idle)
    {
        timerIdle.update(delta);
    }
    else if (stage == Stage::Aim)
    {
        timerAim.update(delta);
    }

    // 更新位置1
    totalDeltaTime += delta;
    switch (stage)
    {
    case Stage::Idle:
        currentPosition.y = basePosition.y + sin(totalDeltaTime * 2 + diffPeriod) * 30;
        break;
    case Stage::Aim:
        currentPosition.x = basePosition.x + rangeRandom(-10, 10);
        break;
    case Stage::Dash:
        currentPosition += velocity * delta;
        if (currentPosition.y >= CharacterManager::Instance()->getPlayer()->getFloorY())
        {
            breakBarb();
        }
        if (currentPosition.y <= 0)
        {
            isValid = false;
        }
        break;
    }
    collisionBox->setPosition(currentPosition);

    // 更新动画1
    currentAnimation = (stage == Stage::Break ? &animationBreak : &animationLoose);
    currentAnimation->setPosition(currentPosition);
    currentAnimation->update(delta);
}

void Barb::render()
{
    currentAnimation->render();
}

void Barb::breakBarb()
{
    if (stage == Stage::Break)
    {
        return;
    }

    stage = Stage::Break;
    collisionBox->setEnabled(false);
    playAudio(_T("barbBreak"), false);
}
