#include "player.h"
#include "resources_manager.h"

#include <cmath>

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
        animationAttackLeft.addFrame(ResourcesManager::Instance()->findImage(_T("player_attack_left")), 5);

        Animation &animationAttackRight = animationAttack.right;
        animationAttackRight.setInterval(0.05f);
        animationAttackRight.setLoop(false);
        animationAttackRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationAttackRight.addFrame(ResourcesManager::Instance()->findImage(_T("player_attack_right")), 5);
    }
    {
        AnimationGroup &animationDead = animationPool["dead"];

        Animation &animationDeadLeft = animationDead.left;
        animationDeadLeft.setInterval(0.1f);
        animationDeadLeft.setLoop(false);
        animationDeadLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDeadLeft.addFrame(ResourcesManager::Instance()->findImage(_T("player_dead_left")), 6);

        Animation &animationDeadRight = animationDead.right;
        animationDeadRight.setInterval(0.1f);
        animationDeadRight.setLoop(false);
        animationDeadRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDeadRight.addFrame(ResourcesManager::Instance()->findImage(_T("player_dead_right")), 6);
    }
    {
        AnimationGroup &animationFall = animationPool["fall"];

        Animation &animationFallLeft = animationFall.left;
        animationFallLeft.setInterval(0.15f);
        animationFallLeft.setLoop(false);
        animationFallLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationFallLeft.addFrame(ResourcesManager::Instance()->findImage(_T("player_fall_left")), 5);

        Animation &animationFallRight = animationFall.right;
        animationFallRight.setInterval(0.15f);
        animationFallRight.setLoop(false);
        animationFallRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationFallRight.addFrame(ResourcesManager::Instance()->findImage(_T("player_fall_right")), 5);
    }
    {
        AnimationGroup &animationIdle = animationPool["idle"];

        Animation &animationIdleLeft = animationIdle.left;
        animationIdleLeft.setInterval(0.15f);
        animationIdleLeft.setLoop(false);
        animationIdleLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationIdleLeft.addFrame(ResourcesManager::Instance()->findImage(_T("player_idle_left")), 5);

        Animation &animationIdleRight = animationIdle.right;
        animationIdleRight.setInterval(0.15f);
        animationIdleRight.setLoop(false);
        animationIdleRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationIdleRight.addFrame(ResourcesManager::Instance()->findImage(_T("player_idle_right")), 5);
    }
    {
        AnimationGroup &animationJump = animationPool["jump"];

        Animation &animationJumpLeft = animationJump.left;
        animationJumpLeft.setInterval(0.15f);
        animationJumpLeft.setLoop(false);
        animationJumpLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationJumpLeft.addFrame(ResourcesManager::Instance()->findImage(_T("player_jump_left")), 5);

        Animation &animationJumpRight = animationJump.right;
        animationJumpRight.setInterval(0.15f);
        animationJumpRight.setLoop(false);
        animationJumpRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationJumpRight.addFrame(ResourcesManager::Instance()->findImage(_T("player_jump_right")), 5);
    }
    {
        AnimationGroup &animationRoll = animationPool["roll"];

        Animation &animationRollLeft = animationRoll.left;
        animationRollLeft.setInterval(0.05f);
        animationRollLeft.setLoop(false);
        animationRollLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRollLeft.addFrame(ResourcesManager::Instance()->findImage(_T("player_roll_left")), 7);

        Animation &animationRollRight = animationRoll.right;
        animationRollRight.setInterval(0.05f);
        animationRollRight.setLoop(false);
        animationRollRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRollRight.addFrame(ResourcesManager::Instance()->findImage(_T("player_roll_right")), 7);
    }
    {
        AnimationGroup &animationRun = animationPool["run"];

        Animation &animationRunLeft = animationRun.left;
        animationRunLeft.setInterval(0.075f);
        animationRunLeft.setLoop(false);
        animationRunLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRunLeft.addFrame(ResourcesManager::Instance()->findImage(_T("player_run_left")), 10);

        Animation &animationRunRight = animationRun.right;
        animationRunRight.setInterval(0.075f);
        animationRunRight.setLoop(false);
        animationRunRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRunRight.addFrame(ResourcesManager::Instance()->findImage(_T("player_run_right")), 10);
    }
    {
        animationSlashUp.setInterval(0.07f);
        animationSlashUp.setLoop(false);
        animationSlashUp.setAnchorMode(Animation::AnchorMode::Centered);
        animationSlashUp.addFrame(ResourcesManager::Instance()->findImage(_T("player_vfx_attack_up")), 5);

        animationSlashDown.setInterval(0.07f);
        animationSlashDown.setLoop(false);
        animationSlashDown.setAnchorMode(Animation::AnchorMode::Centered);
        animationSlashDown.addFrame(ResourcesManager::Instance()->findImage(_T("player_vfx_attack_down")), 5);

        animationSlashLeft.setInterval(0.07f);
        animationSlashLeft.setLoop(false);
        animationSlashLeft.setAnchorMode(Animation::AnchorMode::Centered);
        animationSlashLeft.addFrame(ResourcesManager::Instance()->findImage(_T("player_vfx_attack_left")), 5);

        animationSlashRight.setInterval(0.07f);
        animationSlashRight.setLoop(false);
        animationSlashRight.setAnchorMode(Animation::AnchorMode::Centered);
        animationSlashRight.addFrame(ResourcesManager::Instance()->findImage(_T("player_vfx_attack_rigth")), 5);

        animationJumpVfx.setInterval(0.05f);
        animationJumpVfx.setLoop(false);
        animationJumpVfx.setAnchorMode(Animation::AnchorMode::Centered);
        animationJumpVfx.addFrame(ResourcesManager::Instance()->findImage(_T("player_vfx_jump")), 5);
        animationJumpVfx.setOnFinished([&]()
                                       { isJumpVfxVisible = false; });

        animationLandVfx.setInterval(0.1f);
        animationLandVfx.setLoop(false);
        animationLandVfx.setAnchorMode(Animation::AnchorMode::Centered);
        animationLandVfx.addFrame(ResourcesManager::Instance()->findImage(_T("player_vfx_land")), 2);
        animationLandVfx.setOnFinished([&]()
                                       { isLandVfxVisible = false; });
    }
    {
        // [TODO] 状态机初始化
        
    }
}

void Player::input(const ExMessage &msg)
{
    if (hp < 0)
    {
        return;
    }

    switch (msg.message)
    {
    case WM_KEYDOWN:
    {
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
        }
        break;
    }
    case WM_KEYUP:
    {
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
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        isAttackKeyDown = true;
        updateAttackDir(msg.x, msg.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        isAttackKeyDown = false;
        break;
    }
    case WM_RBUTTONDBLCLK:
    {
        // [TODO] 进入子弹时间
        break;
    }
    case WM_RBUTTONDOWN:
    {
        // [TODO] 退出子弹时间
        break;
    }
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
    else
    {
        attackDir = AttackDir::Up;
    }
}

Player::~Player() = default;