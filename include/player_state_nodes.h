#ifndef _PLAYER_STATE_NODES_H_
#define _PLAYER_STATE_NODES_H_

#include "timer.h"
#include "state_node.h"

// 玩家攻击状态1
class PlayerAttackState : public StateNode
{
public:
    PlayerAttackState();
    ~PlayerAttackState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    Timer timer;

private:
    void updateHitBoxPosition();
};

// 玩家死亡状态1
class PlayerDeadState : public StateNode
{
public:
    PlayerDeadState();
    ~PlayerDeadState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};

// 玩家跌落状态1
class PlayerFallState : public StateNode
{
public:
    PlayerFallState() = default;
    ~PlayerFallState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};

// 玩家空闲状态1
class PlayerIdleState : public StateNode
{
public:
    PlayerIdleState() = default;
    ~PlayerIdleState() = default;

    void enter() override;
    void update(float delta) override;
};

class PlayerJumpState : public StateNode
{
public:
    PlayerJumpState() = default;
    ~PlayerJumpState() = default;

    void enter() override;
    void update(float delta) override;
};

// 玩家翻滚状态1
class PlayerRollState : public StateNode
{
public:
    PlayerRollState();
    ~PlayerRollState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    Timer timer;
};

// 玩家奔跑状态1
class PlayerRunState : public StateNode
{
public:
    PlayerRunState() = default;
    ~PlayerRunState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;
};

#endif // _PLAYER_STATE_NODES_H_