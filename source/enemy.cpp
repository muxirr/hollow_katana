#include "enemy.h"
#include "character_manager.h"
#include "resources_manager.h"
#include "collision_manager.h"
#include "enemy_state_nodes.h"

#include <iostream>

Enemy::Enemy()
{
    isFacingLeft = true;
    position = {1050, 200};
    logicHeight = 150;

    hitBox->setSize({50, 80});
    hurtBox->setSize({100, 180});

    hitBox->setLayerSrc(CollisionLayer::None);
    hitBox->setLayerDst(CollisionLayer::Player);

    hurtBox->setLayerSrc(CollisionLayer::Enemy);
    hurtBox->setLayerDst(CollisionLayer::None);

    hurtBox->setOnCollide([&]()
                          { decreaseHp(); });

    collisionBoxSilk = CollisionManager::Instance()->createCollisionBox();
    collisionBoxSilk->setLayerSrc(CollisionLayer::None);
    collisionBoxSilk->setLayerDst(CollisionLayer::Player);
    collisionBoxSilk->setSize({225, 225});
    collisionBoxSilk->setEnabled(false);

    {
        {
            AnimationGroup &animationAim = animationPool["aim"];

            Animation &animationAimLeft = animationAim.left;
            animationAimLeft.setInterval(0.05f);
            animationAimLeft.setLoop(false);
            animationAimLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationAimLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyAimLeft")));

            Animation &animationAimRight = animationAim.right;
            animationAimRight.setInterval(0.05f);
            animationAimRight.setLoop(false);
            animationAimRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationAimRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyAimRight")));
        }
        {
            AnimationGroup &animationDashInAir = animationPool["dashInAir"];

            Animation &animationDashInAirLeft = animationDashInAir.left;
            animationDashInAirLeft.setInterval(0.05f);
            animationDashInAirLeft.setLoop(true);
            animationDashInAirLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationDashInAirLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyDashInAirLeft")));

            Animation &animationDashInAirRight = animationDashInAir.right;
            animationDashInAirRight.setInterval(0.05f);
            animationDashInAirRight.setLoop(true);
            animationDashInAirRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationDashInAirRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyDashInAirRight")));
        }
        {
            AnimationGroup &animationDashOnFloor = animationPool["dashOnFloor"];

            Animation &animationDashOnFloorLeft = animationDashOnFloor.left;
            animationDashOnFloorLeft.setInterval(0.05f);
            animationDashOnFloorLeft.setLoop(true);
            animationDashOnFloorLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationDashOnFloorLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyDashOnFloorLeft")));

            Animation &animationDashOnFloorRight = animationDashOnFloor.right;
            animationDashOnFloorRight.setInterval(0.05f);
            animationDashOnFloorRight.setLoop(true);
            animationDashOnFloorRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationDashOnFloorRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyDashOnFloorRight")));
        }
        {
            AnimationGroup &animationFall = animationPool["fall"];

            Animation &animationFallLeft = animationFall.left;
            animationFallLeft.setInterval(0.1f);
            animationFallLeft.setLoop(true);
            animationFallLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationFallLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyFallLeft")));

            Animation &animationFallRight = animationFall.right;
            animationFallRight.setInterval(0.1f);
            animationFallRight.setLoop(true);
            animationFallRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationFallRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyFallRight")));
        }
        {
            AnimationGroup &animationIdle = animationPool["idle"];

            Animation &animationIdleLeft = animationIdle.left;
            animationIdleLeft.setInterval(0.1f);
            animationIdleLeft.setLoop(true);
            animationIdleLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationIdleLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyIdleLeft")));

            Animation &animationIdleRight = animationIdle.right;
            animationIdleRight.setInterval(0.1f);
            animationIdleRight.setLoop(true);
            animationIdleRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationIdleRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyIdleRight")));
        }
        {
            AnimationGroup &animationJump = animationPool["jump"];

            Animation &animationJumpLeft = animationJump.left;
            animationJumpLeft.setInterval(0.1f);
            animationJumpLeft.setLoop(false);
            animationJumpLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationJumpLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyJumpLeft")));

            Animation &animationJumpRight = animationJump.right;
            animationJumpRight.setInterval(0.1f);
            animationJumpRight.setLoop(false);
            animationJumpRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationJumpRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyJumpRight")));
        }
        {
            AnimationGroup &animationRun = animationPool["run"];

            Animation &animationRunLeft = animationRun.left;
            animationRunLeft.setInterval(0.05f);
            animationRunLeft.setLoop(true);
            animationRunLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRunLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyRunLeft")));

            Animation &animationRunRight = animationRun.right;
            animationRunRight.setInterval(0.05f);
            animationRunRight.setLoop(true);
            animationRunRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRunRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyRunRight")));
        }
        {
            AnimationGroup &animationSquat = animationPool["squat"];

            Animation &animationSquatLeft = animationSquat.left;
            animationSquatLeft.setInterval(0.05f);
            animationSquatLeft.setLoop(false);
            animationSquatLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationSquatLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemySquatLeft")));

            Animation &animationSquatRight = animationSquat.right;
            animationSquatRight.setInterval(0.05f);
            animationSquatRight.setLoop(false);
            animationSquatRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationSquatRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemySquatRight")));
        }
        {
            AnimationGroup &animationThrowBarb = animationPool["throwBarb"];

            Animation &animationThrowBarbLeft = animationThrowBarb.left;
            animationThrowBarbLeft.setInterval(0.1f);
            animationThrowBarbLeft.setLoop(false);
            animationThrowBarbLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowBarbLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyThrowBarbLeft")));

            Animation &animationThrowBarbRight = animationThrowBarb.right;
            animationThrowBarbRight.setInterval(0.1f);
            animationThrowBarbRight.setLoop(false);
            animationThrowBarbRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowBarbRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyThrowBarbRight")));
        }
        {
            AnimationGroup &animationThrowSilk = animationPool["throwSilk"];

            Animation &animationThrowSilkLeft = animationThrowSilk.left;
            animationThrowSilkLeft.setInterval(0.1f);
            animationThrowSilkLeft.setLoop(true);
            animationThrowSilkLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSilkLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyThrowSilkLeft")));

            Animation &animationThrowSilkRight = animationThrowSilk.right;
            animationThrowSilkRight.setInterval(0.1f);
            animationThrowSilkRight.setLoop(true);
            animationThrowSilkRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSilkRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyThrowSilkRight")));
        }
        {
            AnimationGroup &animationThrowSword = animationPool["throwSword"];

            Animation &animationThrowSwordLeft = animationThrowSword.left;
            animationThrowSwordLeft.setInterval(0.05f);
            animationThrowSwordLeft.setLoop(false);
            animationThrowSwordLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSwordLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyThrowSwordLeft")));

            Animation &animationThrowSwordRight = animationThrowSword.right;
            animationThrowSwordRight.setInterval(0.05f);
            animationThrowSwordRight.setLoop(false);
            animationThrowSwordRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSwordRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyThrowSwordRight")));
        }
    }

    {
        animationSilk.setInterval(0.1f);
        animationSilk.setLoop(false);
        animationSilk.setAnchorMode(Animation::AnchorMode::Centered);
        animationSilk.addFrame(*ResourcesManager::Instance()->findAtlas(_T("silk")));

        Animation &animationDashInAirVfxLeft = animationDashInAirVfx.left;
        animationDashInAirVfxLeft.setInterval(0.1f);
        animationDashInAirVfxLeft.setLoop(false);
        animationDashInAirVfxLeft.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashInAirVfxLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyDashInAirVfxLeft")));

        Animation &animationDashInAirVfxRight = animationDashInAirVfx.right;
        animationDashInAirVfxRight.setInterval(0.1f);
        animationDashInAirVfxRight.setLoop(false);
        animationDashInAirVfxRight.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashInAirVfxRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyDashInAirVfxRight")));

        Animation &animationDashOnFloorVfxLeft = animationDashOnFloorVfx.left;
        animationDashOnFloorVfxLeft.setInterval(0.1f);
        animationDashOnFloorVfxLeft.setLoop(false);
        animationDashOnFloorVfxLeft.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashOnFloorVfxLeft.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyDashOnFloorVfxLeft")));

        Animation &animationDashOnFloorVfxRight = animationDashOnFloorVfx.right;
        animationDashOnFloorVfxRight.setInterval(0.1f);
        animationDashOnFloorVfxRight.setLoop(false);
        animationDashOnFloorVfxRight.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashOnFloorVfxRight.addFrame(*ResourcesManager::Instance()->findAtlas(_T("enemyDashOnFloorVfxRight")));
    }
    {
        // [TODO] 状态机初始化1
        stateMachine.registerState("aim", new EnemyAimState());
        stateMachine.registerState("dashInAir", new EnemyDashInAirState());
        stateMachine.registerState("dashOnFloor", new EnemyDashOnFloorState());
        stateMachine.registerState("dead", new EnemyDeadState());
        stateMachine.registerState("fall", new EnemyFallState());
        stateMachine.registerState("idle", new EnemyIdleState());
        stateMachine.registerState("jump", new EnemyJumpState());
        stateMachine.registerState("run", new EnemyRunState());
        stateMachine.registerState("squat", new EnemySquatState());
        stateMachine.registerState("throwBarb", new EnemyThrowBarbState());
        stateMachine.registerState("throwSilk", new EnemyThrowSilkState());
        stateMachine.registerState("throwSword", new EnemyThrowSwordState());

        stateMachine.setEntry("idle");
    }
}

Enemy::~Enemy()
{
    CollisionManager::Instance()->destroyCollisionBox(collisionBoxSilk);
}

void Enemy::update(float delta)
{
    if (velocity.x >= 0.0001f)
    {
        isFacingLeft = (velocity.x < 0);
    }

    Character::update(delta);

    hitBox->setPosition(getLogicCenter());

    if (isThrowingSlik)
    {
        collisionBoxSilk->setPosition(getLogicCenter());
        animationSilk.setPosition(getLogicCenter());
        animationSilk.update(delta);
    }

    if (isDashInAir || isDashOnFloor)
    {
        currentDashAnimation->setPosition(isDashInAir ? getLogicCenter() : position);
        currentDashAnimation->update(delta);
    }

    for (Barb *barb : barbList)
    {
        barb->update(delta);
    }

    for (Sword *sword : swordList)
    {
        sword->update(delta);
    }

    barbList.erase(std::remove_if(barbList.begin(), barbList.end(),
                                  [](Barb *barb)
                                  {
                                        bool canRemove = !barb->checkValid();
                                        if (canRemove)
                                        {
                                            delete barb;
                                        }
                                        return canRemove; }),
                   barbList.end());
    swordList.erase(std::remove_if(swordList.begin(), swordList.end(),
                                   [](Sword *sword)
                                   {
                                        bool canRemove = !sword->checkValid();
                                        if (canRemove)
                                        {
                                            delete sword;
                                        }
                                        return canRemove; }),
                    swordList.end());
}

void Enemy::render()
{
    for (Barb *barb : barbList)
    {
        barb->render();
    }
    for (Sword *sword : swordList)
    {
        sword->render();
    }

    Character::render();

    if (isThrowingSlik)
    {
        animationSilk.render();
    }

    if (isDashInAir || isDashOnFloor)
    {
        currentDashAnimation->render();
    }
}

void Enemy::hurted()
{
    switch (rangeRandom(1, 3))
    {
    case 1:
        playAudio(_T("enemyHurt1"), false);
        break;
    case 2:
        playAudio(_T("enemyHurt2"), false);
        break;
    case 3:
        playAudio(_T("enemyHurt3"), false);
        break;
    }
}

void Enemy::throwBarbs()
{
    int numNewBarbs = rangeRandom(3, 6);
    if (barbList.size() >= 10)
    {
        numNewBarbs = 1;
    }
    int widthGrid = getwidth() / numNewBarbs;

    for (int i = 0; i < numNewBarbs; i++)
    {
        Barb *barb = new Barb();
        int randX = rangeRandom(i * widthGrid, (i + 1) * widthGrid);
        int randY = rangeRandom(250, 500);
        barb->setPosition({(float)randX, (float)randY});
        barbList.push_back(barb);
    }
}

void Enemy::throwSword()
{
    Sword *sword = new Sword(getLogicCenter(), isFacingLeft);
    swordList.push_back(sword);
}

void Enemy::dash()
{
    if (isDashInAir)
    {
        currentDashAnimation = velocity.x < 0 ? &animationPool["dashInAir"].left : &animationPool["dashInAir"].right;
    }
    else
    {
        currentDashAnimation = velocity.x < 0 ? &animationPool["dashOnFloor"].left : &animationPool["dashOnFloor"].right;
    }

    currentDashAnimation->reset();
}

void Enemy::throwSlik()
{
    animationSilk.reset();
}

void Enemy::log()
{
    std::cout << "\n";
    std::cout << "Enemy: " << hp;
}