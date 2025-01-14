#include "enemy_state_nodes.h"
#include "character_manager.h"
#include "enemy.h"

// 敌人瞄准状态1
EnemyAimState::EnemyAimState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.5f);
    timer.setTimeOut([&]()
                     { Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
                       enemy->setGravityEnable(true);
                       enemy->switchState("dashInAir"); });
}

void EnemyAimState::enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
    enemy->setGravityEnable(false);
    enemy->setVelocity({0.0f, 0.0f});
    timer.restart();
}

void EnemyAimState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
    timer.update(delta);

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
}

// 敌人冲刺状态1
void EnemyDashInAirState::enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    enemy->setAnimation("dashInAir");
    const Character *player = CharacterManager::Instance()->getPlayer();
    Vector2 posTarget = {player->getPosition().x, player->getFloorY()};
    enemy->setVelocity((posTarget - enemy->getPosition()).normalize() * DASH_SPEED);
    enemy->setDashInAir(true);
    enemy->setGravityEnable(false);
    enemy->dash();

    playAudio(_T("enemyDash"), false);
}

void EnemyDashInAirState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
    else if (enemy->isOnFloor())
    {
        enemy->switchState("idle");
    }
}

void EnemyDashInAirState::exit()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    enemy->setGravityEnable(true);
    enemy->setDashInAir(false);
}

EnemyDashOnFloorState::EnemyDashOnFloorState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.5f);
    timer.setTimeOut([&]()
                     { Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
                       enemy->setDashOnFloor(false); });
}

void EnemyDashOnFloorState::enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    enemy->setAnimation("dashOnFloor");
    enemy->setVelocity({enemy->getFacingLeft() ? -DASH_SPEED : DASH_SPEED, 0.0f});
    enemy->setDashOnFloor(true);
    enemy->dash();
    timer.restart();

    playAudio(_T("enemyDash"), false);
}

void EnemyDashOnFloorState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    timer.update(delta);

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
    else if (enemy->getDashOnFloor() == false)
    {
        enemy->switchState("idle");
    }
}

// 敌人死亡状态1
void EnemyDeadState::enter()
{
    MessageBox(GetHWnd(), _T("You Win!"), _T("Game Over"), MB_OK);
    std::exit(0);
}

// 敌人下落状态1
void EnemyFallState::enter()
{
    CharacterManager::Instance()->getEnemy()->setAnimation("fall");
}

void EnemyFallState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
    else if (enemy->isOnFloor())
    {
        enemy->switchState("idle");
    }
}

// 敌人闲置状态1
EnemyIdleState::EnemyIdleState()
{
    timer.setOneShot(true);
    timer.setTimeOut([&]()
                     {
        Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

        int randNum = rangeRandom(0, 100);
        if (enemy->getHp() > 5)
        {
            if (randNum <= 25)
            {
                if (!enemy->isOnFloor())
                {
                    enemy->switchState("fall");
                }
                else
                {
                    enemy->switchState("jump");
                }
            }
            else if (randNum <= 50)
            {
                if (!enemy->isOnFloor())
                {
                    enemy->switchState("fall");
                }
                else
                {
                    enemy->switchState("run");
                }
            }
            else if (randNum <= 80)
            {
                enemy->switchState("squat");
            }
            else if (randNum <= 90)
            {
                enemy->switchState("throwSilk");
            }
            else
            {
                enemy->switchState("throwSword");
            }
        }
        else
        {
            if (randNum <= 25)
            {
                if (!enemy->isOnFloor())
                {
                    enemy->switchState("fall");
                }
                else
                {
                    enemy->switchState("jump");
                }
            }
            else if (randNum <= 60)
            {
                enemy->switchState("throwSword");
            }
            else if (randNum <= 70)
            {
                enemy->switchState("throwSilk");
            }
            else if(randNum <= 90)
            {
                enemy->switchState("throwBarb");
            }else
            {
                enemy->switchState("squat");
            }
        } });
}

void EnemyIdleState::enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    enemy->setVelocity({0.0f, 0.0f});

    float waitTime = 0;
    if (enemy->getHp() > 5)
    {
        waitTime = rangeRandom(0, 2) * 0.25f; // 0.0s ~ 0.5s
    }
    else
    {
        waitTime = rangeRandom(0, 1) * 0.25f; // 0.0s ~ 0.25s
    }

    timer.setWaitTime(waitTime);
    timer.restart();
}

void EnemyIdleState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    timer.update(delta);

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
    else if (enemy->getVelocity().y > 0)
    {
        enemy->switchState("fall");
    }
}

void EnemyIdleState::exit()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    enemy->setFacingLeft(enemy->getPosition().x > CharacterManager::Instance()->getPlayer()->getPosition().x);
}

// 敌人跳跃状态1
void EnemyJumpState::enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    enemy->setAnimation("jump");
    enemy->setVelocity({0.0f, -JUMP_SPEED});
}

void EnemyJumpState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
    else if (enemy->getVelocity().y > 0)
    {
        int randNum = rangeRandom(0, 100);
        if (enemy->getHp() > 5)
        {
            if (randNum <= 50)
            {
                enemy->switchState("aim");
            }
            else if (randNum <= 80)
            {
                enemy->switchState("fall");
            }
            else
            {
                enemy->switchState("throwSilk");
            }
        }
        else
        {
            if (randNum <= 50)
            {
                enemy->switchState("throwSilk");
            }
            else if (randNum <= 80)
            {
                enemy->switchState("fall");
            }
            else
            {
                enemy->switchState("aim");
            }
        }
    }
}

// 敌人奔跑状态1
void EnemyRunState::enter()
{
    CharacterManager::Instance()->getEnemy()->setAnimation("run");
    playAudio(_T("enemyRun"), true);
}

void EnemyRunState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    const Vector2 &posPlayer = CharacterManager::Instance()->getPlayer()->getPosition();
    const Vector2 &posEnemy = CharacterManager::Instance()->getEnemy()->getPosition();
    enemy->setVelocity({posPlayer.x > posEnemy.x ? RUN_SPEED : -RUN_SPEED, 0.0f});

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
    else if (abs(posEnemy.x - posPlayer.x) < MIN_DIS)
    {
        int randNum = rangeRandom(0, 100);
        if (enemy->getHp() > 5)
        {
            if (randNum <= 75)
            {
                enemy->switchState("squat");
            }
            else
            {
                enemy->switchState("throwSilk");
            }
        }
        else
        {
            if (randNum <= 75)
            {
                enemy->switchState("throwSilk");
            }
            else
            {
                enemy->switchState("squat");
            }
            stopAudio(_T("enemyRun"));
        }
    }
}

void EnemyRunState::exit()
{
    CharacterManager::Instance()->getEnemy()->setVelocity({0.0f, 0.0f});
}

// 敌人蹲下状态1
EnemySquatState::EnemySquatState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.5f);
    timer.setTimeOut([&]()
                     {
                        Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
                        enemy->switchState("dashOnFloor"); });
}

void EnemySquatState::enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    enemy->setAnimation("squat");
    enemy->setFacingLeft(enemy->getPosition().x > CharacterManager::Instance()->getPlayer()->getPosition().x);
    timer.restart();
}

void EnemySquatState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    timer.update(delta);

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
}

// 敌人扔刺球状态1
EnemyThrowBarbState::EnemyThrowBarbState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.8f);
    timer.setTimeOut([&]()
                     {
                        Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
                        enemy->throwBarbs();
                        enemy->switchState("idle"); });
}

void EnemyThrowBarbState::enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    enemy->setAnimation("throwBarb");
    timer.restart();

    playAudio(_T("enemyThrowBarb"), false);
}

void EnemyThrowBarbState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    timer.update(delta);

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
}

// 敌人扔线状态1
EnemyThrowSilkState::EnemyThrowSilkState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.9f);
    timer.setTimeOut([&]()
                     {
                        Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
                        enemy->setGravityEnable(true);
                        enemy->setThrowingSlik(false);
                        if(!enemy->isOnFloor() && enemy->getHp() > 5 && rangeRandom(0,100) <= 25){
                            enemy->switchState("aim");
                        }else if(!enemy->isOnFloor()){
                            enemy->switchState("fall");
                        }else{
                            enemy->switchState("idle");
                        } });
}

void EnemyThrowSilkState::enter()
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    enemy->setAnimation("throwSilk");
    enemy->setGravityEnable(false);
    enemy->setThrowingSlik(true);
    enemy->setVelocity({0.0f, 0.0f});
    enemy->throwSlik();
    timer.restart();

    playAudio(_T("enemyThrowSilk"), false);
}

void EnemyThrowSilkState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    timer.update(delta);

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
}

// 敌人扔剑状态1
EnemyThrowSwordState::EnemyThrowSwordState()
{
    timerThrow.setOneShot(true);
    timerThrow.setWaitTime(0.65f);
    timerThrow.setTimeOut([&]()
                          {
                        Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
                        enemy->throwSword();
                        playAudio(_T("enemyThrowSword"), false); });

    timerSwitch.setOneShot(true);
    timerSwitch.setWaitTime(1.0f);
    timerSwitch.setTimeOut([&]()
                           {
                        Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();
                        int randNum = rangeRandom(0, 100);
                        if(enemy->getHp() >5){
                            if(randNum <= 50){
                                enemy->switchState("squat");
                            }else if(randNum <= 80){
                                enemy->switchState("jump");
                            }else{
                                enemy->switchState("idle");
                            }
                        }else{
                            if(randNum <= 50){
                                enemy->switchState("jump");
                            }else if(randNum <= 80){
                                enemy->switchState("throwSilk");
                            }else{
                                enemy->switchState("idle");
                            }
                        } });
}

void EnemyThrowSwordState::enter()
{
    CharacterManager::Instance()->getEnemy()->setAnimation("throwSword");

    timerThrow.restart();
    timerSwitch.restart();
}

void EnemyThrowSwordState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::Instance()->getEnemy();

    timerThrow.update(delta);
    timerSwitch.update(delta);

    if (enemy->getHp() <= 0)
    {
        enemy->switchState("dead");
    }
}