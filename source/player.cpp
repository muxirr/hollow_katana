#include "player.h"
#include "resources_manager.h"
#include "player_state_nodes.h"
#include "bullet_time_manager.h"

#include <cmath>
#include <iostream>

Player::Player()
{
    isFacingLeft = false;
    position = Vector2(250, 200);
    logicHeight = 120;

    hitBox->setSize({150, 150});
    hurtBox->setSize({40, 80});

    hitBox->setLayerSrc(CollisionLayer::None);
    hitBox->setLayerDst(CollisionLayer::Enemy);

    hurtBox->setLayerSrc(CollisionLayer::Player);
    hurtBox->setLayerDst(CollisionLayer::None);

    hitBox->setEnabled(false);
    hurtBox->setOnCollide([&]()
                          { decreaseHp(); });

    timerRollCd.setWaitTime(ROLL_CD);
    timerRollCd.setOneShot(true);
    timerRollCd.setTimeOut([&]()
                           { isRollCdComp = true; });

    timerAttackCd.setWaitTime(ATTACK_CD);
    timerAttackCd.setOneShot(true);
    timerAttackCd.setTimeOut([&]()
                             { isAttackCdComp = true; });
    {
        AnimationGroup &animationAttack = animationPool["attack"];

        Animation &animationAttackLeft = animationAttack.left;
        animationAttackLeft.setInterval(0.05f);
        animationAttackLeft.setLoop(false);
        animationAttackLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationAttackLeft.addFrame(ResourcesManager::Instance()->findImage(_T("playerAttackLeft")), 5);

        Animation &animationAttackRight = animationAttack.right;
        animationAttackRight.setInterval(0.05f);
        animationAttackRight.setLoop(false);
        animationAttackRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationAttackRight.addFrame(ResourcesManager::Instance()->findImage(_T("playerAttackRight")), 5);
    }
    {
        AnimationGroup &animationDead = animationPool["dead"];

        Animation &animationDeadLeft = animationDead.left;
        animationDeadLeft.setInterval(0.1f);
        animationDeadLeft.setLoop(false);
        animationDeadLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDeadLeft.addFrame(ResourcesManager::Instance()->findImage(_T("playerDeadLeft")), 6);

        Animation &animationDeadRight = animationDead.right;
        animationDeadRight.setInterval(0.1f);
        animationDeadRight.setLoop(false);
        animationDeadRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDeadRight.addFrame(ResourcesManager::Instance()->findImage(_T("playerDeadRight")), 6);
    }
    {
        AnimationGroup &animationFall = animationPool["fall"];

        Animation &animationFallLeft = animationFall.left;
        animationFallLeft.setInterval(0.15f);
        animationFallLeft.setLoop(true);
        animationFallLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationFallLeft.addFrame(ResourcesManager::Instance()->findImage(_T("playerFallLeft")), 5);

        Animation &animationFallRight = animationFall.right;
        animationFallRight.setInterval(0.15f);
        animationFallRight.setLoop(true);
        animationFallRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationFallRight.addFrame(ResourcesManager::Instance()->findImage(_T("playerFallRight")), 5);
    }
    {
        AnimationGroup &animationIdle = animationPool["idle"];

        Animation &animationIdleLeft = animationIdle.left;
        animationIdleLeft.setInterval(0.15f);
        animationIdleLeft.setLoop(true);
        animationIdleLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationIdleLeft.addFrame(ResourcesManager::Instance()->findImage(_T("playerIdleLeft")), 5);

        Animation &animationIdleRight = animationIdle.right;
        animationIdleRight.setInterval(0.15f);
        animationIdleRight.setLoop(true);
        animationIdleRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationIdleRight.addFrame(ResourcesManager::Instance()->findImage(_T("playerIdleRight")), 5);
    }
    {
        AnimationGroup &animationJump = animationPool["jump"];

        Animation &animationJumpLeft = animationJump.left;
        animationJumpLeft.setInterval(0.15f);
        animationJumpLeft.setLoop(false);
        animationJumpLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationJumpLeft.addFrame(ResourcesManager::Instance()->findImage(_T("playerJumpLeft")), 5);

        Animation &animationJumpRight = animationJump.right;
        animationJumpRight.setInterval(0.15f);
        animationJumpRight.setLoop(false);
        animationJumpRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationJumpRight.addFrame(ResourcesManager::Instance()->findImage(_T("playerJumpRight")), 5);
    }
    {
        AnimationGroup &animationRoll = animationPool["roll"];

        Animation &animationRollLeft = animationRoll.left;
        animationRollLeft.setInterval(0.05f);
        animationRollLeft.setLoop(false);
        animationRollLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRollLeft.addFrame(ResourcesManager::Instance()->findImage(_T("playerRollLeft")), 7);

        Animation &animationRollRight = animationRoll.right;
        animationRollRight.setInterval(0.05f);
        animationRollRight.setLoop(false);
        animationRollRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRollRight.addFrame(ResourcesManager::Instance()->findImage(_T("playerRollRight")), 7);
    }
    {
        AnimationGroup &animationRun = animationPool["run"];

        Animation &animationRunLeft = animationRun.left;
        animationRunLeft.setInterval(0.075f);
        animationRunLeft.setLoop(true);
        animationRunLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRunLeft.addFrame(ResourcesManager::Instance()->findImage(_T("playerRunLeft")), 10);

        Animation &animationRunRight = animationRun.right;
        animationRunRight.setInterval(0.075f);
        animationRunRight.setLoop(true);
        animationRunRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRunRight.addFrame(ResourcesManager::Instance()->findImage(_T("playerRunRight")), 10);
    }
    {
        animationSlashUp.setInterval(0.07f);
        animationSlashUp.setLoop(false);
        animationSlashUp.setAnchorMode(Animation::AnchorMode::Centered);
        animationSlashUp.addFrame(ResourcesManager::Instance()->findImage(_T("playerVfxAttackUp")), 5);

        animationSlashDown.setInterval(0.07f);
        animationSlashDown.setLoop(false);
        animationSlashDown.setAnchorMode(Animation::AnchorMode::Centered);
        animationSlashDown.addFrame(ResourcesManager::Instance()->findImage(_T("playerVfxAttackDown")), 5);

        animationSlashLeft.setInterval(0.07f);
        animationSlashLeft.setLoop(false);
        animationSlashLeft.setAnchorMode(Animation::AnchorMode::Centered);
        animationSlashLeft.addFrame(ResourcesManager::Instance()->findImage(_T("playerVfxAttackLeft")), 5);

        animationSlashRight.setInterval(0.07f);
        animationSlashRight.setLoop(false);
        animationSlashRight.setAnchorMode(Animation::AnchorMode::Centered);
        animationSlashRight.addFrame(ResourcesManager::Instance()->findImage(_T("playerVfxAttackRight")), 5);

        animationJumpVfx.setInterval(0.05f);
        animationJumpVfx.setLoop(false);
        animationJumpVfx.setAnchorMode(Animation::AnchorMode::Centered);
        animationJumpVfx.addFrame(ResourcesManager::Instance()->findImage(_T("playerVfxJump")), 5);
        animationJumpVfx.setOnFinished([&]()
                                       { isJumpVfxVisible = false; });

        animationLandVfx.setInterval(0.1f);
        animationLandVfx.setLoop(false);
        animationLandVfx.setAnchorMode(Animation::AnchorMode::Centered);
        animationLandVfx.addFrame(ResourcesManager::Instance()->findImage(_T("playerVfxLand")), 2);
        animationLandVfx.setOnFinished([&]()
                                       { isLandVfxVisible = false; });
    }
    {
        // 结尾不加1会编译报错，微软你tm还不修复这个bug？
        // [TODO] 状态机初始化1
        stateMachine.registerState("attack", new PlayerAttackState());
        stateMachine.registerState("dead", new PlayerDeadState());
        stateMachine.registerState("fall", new PlayerFallState());
        stateMachine.registerState("idle", new PlayerIdleState());
        stateMachine.registerState("jump", new PlayerJumpState());
        stateMachine.registerState("roll", new PlayerRollState());
        stateMachine.registerState("run", new PlayerRunState());

        stateMachine.setEntry("idle");
    }
}

Player::~Player() = default;

void Player::input(const ExMessage &msg)
{
    if (hp < 0)
    {
        return;
    }

    switch (msg.message)
    {
    case WM_KEYDOWN:

        switch (msg.vkcode)
        {
        case 0x41: // A
        case VK_LEFT:
            isLeftKeyDown = true;
            break;
        case 0x44: // D
        case VK_RIGHT:
            isRightKeyDown = true;
            break;
        case 0x57: // W
        case VK_SPACE:
        case VK_UP:
            isJumpKeyDown = true;
            break;
        case 0x53: // S
        case VK_DOWN:
            isRollKeyDown = true;
            break;
        case 0x4A: // j
            isAttackKeyDown = true;
        }
        break;

    case WM_KEYUP:

        switch (msg.vkcode)
        {
        case 0x41: // A
        case VK_LEFT:
            isLeftKeyDown = false;
            break;
        case 0x44: // D
        case VK_RIGHT:
            isRightKeyDown = false;
            break;
        case 0x57: // W
        case VK_SPACE:
        case VK_UP:
            isJumpKeyDown = false;
            break;
        case 0x53: // S
        case VK_DOWN:
            isRollKeyDown = false;
            break;
        case 0x4A: // j
            isAttackKeyDown = false;
        }
        break;

    case WM_LBUTTONDOWN:

        isAttackKeyDown = true;
        updateAttackDir(msg.x, msg.y);
        break;

    case WM_LBUTTONUP:

        isAttackKeyDown = false;
        break;

    case WM_RBUTTONDOWN:

        // [TODO] 进入子弹时间1
        playAudio(_T("bulletTime"), false);
        BulletTimeManager::Instance()->setStatus(BulletTimeManager::Status::Entering);
        break;

    case WM_RBUTTONUP:

        // [TODO] 退出子弹时间1
        playAudio(_T("bulletTime"), false);
        BulletTimeManager::Instance()->setStatus(BulletTimeManager::Status::Exiting);

        break;
    }
}

void Player::update(float delta)
{
    if (hp > 0 && !isRolling)
    {
        velocity.x = getMoveAxis() * RUN_SPEED;
    }

    if (getMoveAxis() != 0)
    {
        isFacingLeft = getMoveAxis() < 0;
    }

    timerRollCd.update(delta);
    timerAttackCd.update(delta);

    animationJumpVfx.update(delta);
    animationLandVfx.update(delta);

    if (isAttacking)
    {
        currentSlashAnimation->setPosition(position);
        currentSlashAnimation->update(delta);
    }

    Character::update(delta);
}

void Player::render()
{
    if (isJumpVfxVisible)
    {
        animationJumpVfx.render();
    }

    if (isLandVfxVisible)
    {
        animationLandVfx.render();
    }

    Character::render();

    if (isAttacking)
    {
        currentSlashAnimation->render();
    }
}

void Player::hurted()
{
    playAudio(_T("player_hurted"), false);
}

void Player::jump()
{
    velocity.y = -JUMP_SPEED;
    isJumpVfxVisible = true;
    animationJumpVfx.setPosition(position);
    animationJumpVfx.reset();
}

void Player::land()
{
    isLandVfxVisible = true;
    animationLandVfx.setPosition(position);
    animationLandVfx.reset();
}

void Player::roll()
{
    timerRollCd.restart();
    isRollCdComp = false;
    velocity.x = isFacingLeft ? -ROLL_SPEED : ROLL_SPEED;
}

void Player::attack()
{
    timerAttackCd.restart();
    isAttackCdComp = false;

    switch (attackDir)
    {
    case AttackDir::Up:
        currentSlashAnimation = &animationSlashUp;
        break;
    case AttackDir::Down:
        currentSlashAnimation = &animationSlashDown;
        break;
    case AttackDir::Left:
        currentSlashAnimation = &animationSlashLeft;
        break;
    case AttackDir::Right:
        currentSlashAnimation = &animationSlashRight;
        break;
    }

    currentSlashAnimation->setPosition(getLogicCenter());
    currentSlashAnimation->reset();
}

void Player::updateAttackDir(int x, int y)
{
    static const float PI = 3.14159265358979323846f;
    float angle = std::atan2(y - position.y, x - position.x);

    if (angle >= -PI / 4 && angle <= PI / 4)
    {
        attackDir = AttackDir::Right;
    }
    else if (angle >= PI / 4 && angle <= 3 * PI / 4)
    {
        attackDir = AttackDir::Down;
    }
    else if ((angle >= 3 * PI / 4 && angle <= PI) || (angle >= -PI && angle <= -3 * PI / 4))
    {
        attackDir = AttackDir::Left;
    }
    else if (angle >= -3 * PI / 4 && angle <= -PI / 4)
    {
        attackDir = AttackDir::Up;
    }
}

void Player::log()
{
    // std::cout << "Player: " << hp << std::endl;
    // std::cout << "Position: " << position.x << ", " << position.y << std::endl;
    std::cout << "Velocity: " << velocity.x << ", " << velocity.y << std::endl;
    std::cout << "Facing: " << (isFacingLeft ? "Left" : "Right") << std::endl;
    std::cout << "OnFloor: " << (isOnFloor() ? "Yes" : "No") << std::endl;
    // std::cout << "MoveAxis: " << getMoveAxis() << std::endl;
    std::cout << "A: " << isLeftKeyDown << " D: " << isRightKeyDown << " W: " << isJumpKeyDown << " S: " << isRollKeyDown << " J: " << isAttackKeyDown << std::endl;
    std::cout << "Rolling: " << isRolling << " RollCdComp: " << isRollCdComp << " Attacking: " << isAttacking << " AttackCdComp: " << isAttackCdComp << std::endl;
    // std::cout << "Rolling: " << isRolling << std::endl;
    // std::cout << "RollCdComp: " << isRollCdComp << std::endl;
    // std::cout << "Attacking: " << isAttacking << std::endl;
    // std::cout << "AttackCdComp: " << isAttackCdComp << std::endl;
    std::cout << "JumpVfxVisible: " << isJumpVfxVisible << std::endl;
    std::cout << "LandVfxVisible: " << isLandVfxVisible << std::endl;
    std::cout << "\n";
}