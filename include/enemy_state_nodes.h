#ifndef _ENEMY_STATE_NODES_H_
#define _ENEMY_STATE_NODES_H_

#include "timer.h"
#include "state_node.h"

// 敌人瞄准状态1
class EnemyAimState : public StateNode
{
public:
    EnemyAimState();
    ~EnemyAimState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};

// 敌人冲刺状态1
class EnemyDashInAirState : public StateNode
{
public:
    EnemyDashInAirState() = default;
    ~EnemyDashInAirState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    const float DASH_SPEED = 1500.0f;
};

class EnemyDashOnFloorState : public StateNode
{
public:
    EnemyDashOnFloorState();
    ~EnemyDashOnFloorState() = default;

    void enter() override;
    void update(float delta) override;

private:
    const float DASH_SPEED = 1000.0f;

private:
    Timer timer;
};

// 敌人死亡状态1
class EnemyDeadState : public StateNode
{
public:
    EnemyDeadState() = default;
    ~EnemyDeadState() = default;

    void enter() override;
};

// 敌人下落状态1
class EnemyFallState : public StateNode
{
public:
    EnemyFallState() = default;
    ~EnemyFallState() = default;

    void enter() override;
    void update(float delta) override;
};

// 敌人闲置状态1
class EnemyIdleState : public StateNode
{
public:
    EnemyIdleState();
    ~EnemyIdleState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    Timer timer;
};

// 敌人跳跃状态1
class EnemyJumpState : public StateNode
{
public:
    EnemyJumpState() = default;
    ~EnemyJumpState() = default;

    void enter() override;
    void update(float delta) override;

private:
    const float JUMP_SPEED = 1000.0f;
};

// 敌人奔跑状态1
class EnemyRunState : public StateNode
{
public:
    EnemyRunState() = default;
    ~EnemyRunState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    const float MIN_DIS = 350.0f;
    const float RUN_SPEED = 500.0f;
};

// 敌人蹲下状态1
class EnemySquatState : public StateNode
{
public:
    EnemySquatState();
    ~EnemySquatState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};

// 敌人扔刺球状态1
class EnemyThrowBarbState : public StateNode
{
public:
    EnemyThrowBarbState();
    ~EnemyThrowBarbState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};

// 敌人扔线状态1
class EnemyThrowSilkState : public StateNode
{
public:
    EnemyThrowSilkState();
    ~EnemyThrowSilkState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};

// 敌人扔剑状态1
class EnemyThrowSwordState : public StateNode
{
public:
    EnemyThrowSwordState();
    ~EnemyThrowSwordState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timerThrow;
    Timer timerSwitch;
};
#endif // _ENEMY_STATE_NODES_H_