#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"

class Player : public Character
{
public:
    enum class AttackDir
    {
        Up,
        Down,
        Left,
        Right
    };

public:
    Player();
    ~Player();

    void input(const ExMessage &msg) override;
    void update(float delta) override;
    void render() override;

    void hurted();

    void jump();
    void land();
    void roll();
    void attack();

    void setRolling(bool isRolling)
    {
        this->isRolling = isRolling;
    }

    bool getRolling() const
    {
        return isRolling;
    }

    bool canRoll() const
    {
        return isRollCdComp && !isRolling && isRollKeyDown;
    }

    void setAttacking(bool isAttacking)
    {
        this->isAttacking = isAttacking;
    }

    bool getAttacking() const
    {
        return isAttacking;
    }

    bool canAttack() const
    {
        return isAttackCdComp && !isAttacking && isAttackKeyDown;
    }

    bool canJump() const
    {
        return isOnFloor() && isJumpKeyDown;
    }

    int getMoveAxis() const
    {
        return (isRightKeyDown - isLeftKeyDown);
    }

    AttackDir getAttackDir() const
    {
        return attackDir;
    }

private:
    Timer timerRollCd;
    bool isRolling = false;
    bool isRollCdComp = true;

    Timer timerAttackCd;
    bool isAttacking = false;
    bool isAttackCdComp = true;

    bool isLeftKeyDown = false;
    bool isRightKeyDown = false;
    bool isJumpKeyDown = false;
    bool isRollKeyDown = false;
    bool isAttackKeyDown = false;

    Animation animationSlashUp;
    Animation animationSlashDown;
    Animation animationSlashLeft;
    Animation animationSlashRight;
    AttackDir attackDir = AttackDir::Right;
    Animation *currentSlashAnimation = nullptr;

    bool isJumpVfxVisible = false;
    Animation animationJumpVfx;

    bool isLandVfxVisible = false;
    Animation animationLandVfx;

private:
    const float ROLL_CD = 0.75f;
    const float ATTACK_CD = 0.5f;
    const float RUN_SPEED = 300.0f;
    const float JUMP_SPEED = 780.0f;
    const float ROLL_SPEED = 800.0f;

private:
    void updateAttackDir(int x, int y);
};

#endif // _PLAYER_H_