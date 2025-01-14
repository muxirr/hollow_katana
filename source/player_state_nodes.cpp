#include "player.h"
#include "character_manager.h"
#include "player_state_nodes.h"
#include "util.h"

// 玩家攻击状态1
PlayerAttackState::PlayerAttackState()
{
    timer.setWaitTime(0.3f);
    timer.setOneShot(true);
    timer.setTimeOut([&]()
                     { Player *player = (Player *)CharacterManager::Instance()->getPlayer();
                     player->setAttacking(false); });
}

void PlayerAttackState::enter()
{
    Player *player = (Player *)CharacterManager::Instance()->getPlayer();

    player->setAnimation("attack");
    player->getHitBox()->setEnabled(true);
    player->setAttacking(true);
    updateHitBoxPosition();
    player->attack();
    timer.restart();

    switch (range_random(1, 3))
    {
    case 1:
        playAudio(_T("playerAttack1"), false);
        break;
    case 2:
        playAudio(_T("playerAttack2"), false);
        break;
    case 3:
        playAudio(_T("playerAttack3"), false);
        break;
    }
}

void PlayerAttackState::update(float delta)
{
    timer.update(delta);
    updateHitBoxPosition();

    Player *player = (Player *)CharacterManager::Instance()->getPlayer();

    if (player->getHp() <= 0)
    {
        player->switchState("dead");
    }
    else if (!player->getAttacking())
    {
        if (player->getVelocity().y > 0)
        {
            player->switchState("fall");
        }
        else if (player->getMoveAxis() == 0)
        {
            player->switchState("idle");
        }
        else if (player->isOnFloor() && player->getMoveAxis() != 0)
        {
            player->switchState("run");
        }
    }
}

void PlayerAttackState::exit()
{
    Player *player = (Player *)CharacterManager::Instance()->getPlayer();
    player->getHitBox()->setEnabled(false);
    player->setAttacking(false);
}

void PlayerAttackState::updateHitBoxPosition()
{
    Player *player = (Player *)CharacterManager::Instance()->getPlayer();
    Vector2 posCenter = player->getLogicCenter();
    CollisionBox *hitBox = player->getHitBox();
    const Vector2 &sizeHitBox = hitBox->getSize();
    Vector2 posHitBox;
    switch (player->getAttackDir())
    {
    case Player::AttackDir::Up:
        posHitBox = Vector2(posCenter.x, posCenter.y - sizeHitBox.y / 2);
        break;
    case Player::AttackDir::Down:
        posHitBox = Vector2(posCenter.x, posCenter.y + sizeHitBox.y / 2);
        break;
    case Player::AttackDir::Left:
        posHitBox = Vector2(posCenter.x - sizeHitBox.x / 2, posCenter.y);
        break;
    case Player::AttackDir::Right:
        posHitBox = Vector2(posCenter.x + sizeHitBox.x / 2, posCenter.y);
        break;
    }
    hitBox->setPosition(posHitBox);
}

// 玩家死亡状态1
PlayerDeadState::PlayerDeadState()
{
    timer.setWaitTime(1.0f);
    timer.setOneShot(true);
    timer.setTimeOut([&]()
                     { MessageBox(nullptr, _T("You are dead!"), _T("Game Over"), MB_OK);
                     std::exit(0); });
}

void PlayerDeadState::enter()
{
    CharacterManager::Instance()->getPlayer()->setAnimation("dead");

    playAudio(_T("playerDead"), false);
}

void PlayerDeadState::update(float delta)
{
    timer.update(delta);
}

// 玩家跌落状态1
void PlayerFallState::enter()
{
    CharacterManager::Instance()->getPlayer()->setAnimation("fall");
}

void PlayerFallState::update(float delta)
{
    Player *player = (Player *)CharacterManager::Instance()->getPlayer();

    if (player->getHp() <= 0)
    {
        player->switchState("dead");
    }
    else if (player->isOnFloor())
    {
        player->switchState("idle");
        player->land();

        playAudio(_T("playerLand"), false);
    }
    else if (player->canAttack())
    {
        player->switchState("attack");
    }
}

// 玩家空闲状态1
void PlayerIdleState::enter()
{
    CharacterManager::Instance()->getPlayer()->setAnimation("idle");
}

void PlayerIdleState::update(float delta)
{
    Player *player = (Player *)CharacterManager::Instance()->getPlayer();

    if (player->getHp() <= 0)
    {
        player->switchState("dead");
    }
    else if (player->canAttack())
    {
        player->switchState("attack");
    }
    else if (player->getVelocity().y > 0)
    {
        player->switchState("fall");
    }
    else if (player->canJump())
    {
        player->switchState("jump");
    }
    else if (player->canRoll())
    {
        player->switchState("roll");
    }
    else if (player->getMoveAxis() != 0 && player->isOnFloor())
    {
        player->switchState("run");
    }
}

// 玩家跳跃状态1
void PlayerJumpState::enter()
{
    CharacterManager::Instance()->getPlayer()->setAnimation("jump");

    Player *player = (Player *)CharacterManager::Instance()->getPlayer();
    player->jump();

    playAudio(_T("playerJump"), false);
}

void PlayerJumpState::update(float delta)
{
    Player *player = (Player *)CharacterManager::Instance()->getPlayer();

    if (player->getHp() <= 0)
    {
        player->switchState("dead");
    }
    else if (player->getVelocity().y > 0)
    {
        player->switchState("fall");
    }
    else if (player->canAttack())
    {
        player->switchState("attack");
    }
}

// 玩家翻滚状态1
PlayerRollState::PlayerRollState()
{
    timer.setWaitTime(0.35f);
    timer.setOneShot(true);
    timer.setTimeOut([&]()
                     { Player *player = (Player *)CharacterManager::Instance()->getPlayer();
                     player->setRolling(false); });
}

void PlayerRollState::enter()
{
    CharacterManager::Instance()->getPlayer()->setAnimation("roll");

    Player *player = (Player *)CharacterManager::Instance()->getPlayer();
    player->getHurtBox()->setEnabled(false);
    player->setRolling(true);
    player->roll();
    timer.restart();

    playAudio(_T("playerRoll"), false);
}

void PlayerRollState::update(float delta)
{
    timer.update(delta);

    Player *player = (Player *)CharacterManager::Instance()->getPlayer();

    if (!player->getRolling())
    {
        if (player->getMoveAxis() != 0)
        {
            player->switchState("run");
        }
        else if (player->canJump())
        {
            player->switchState("jump");
        }
        else if (player->isOnFloor() && player->getMoveAxis() == 0)
        {
            player->switchState("idle");
        }
    }
}

void PlayerRollState::exit()
{
    CharacterManager::Instance()->getPlayer()->getHurtBox()->setEnabled(true);
}

// 玩家奔跑状态1
void PlayerRunState::enter()
{
    CharacterManager::Instance()->getPlayer()->setAnimation("run");

    playAudio(_T("playerRun"), true);
}

void PlayerRunState::update(float delta)
{
    Player *player = (Player *)CharacterManager::Instance()->getPlayer();

    if (player->getHp() <= 0)
    {
        player->switchState("dead");
    }
    else if (player->canAttack())
    {
        player->switchState("attack");
    }
    else if (player->canJump())
    {
        player->switchState("jump");
    }
    else if (player->canRoll())
    {
        player->switchState("roll");
    }
    else if (player->getMoveAxis() == 0)
    {
        player->switchState("idle");
    }
}

void PlayerRunState::exit()
{
    stopAudio(_T("playerRun"));
}