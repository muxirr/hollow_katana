#include "state_machine.h"

StateMachine::StateMachine() = default;

StateMachine::~StateMachine() = default;

void StateMachine::update(float delta)
{
    if (!currentState)
    {
        return;
    }

    if (needInit)
    {
        currentState->enter();
        needInit = false;
    }

    currentState->update(delta);
}

void StateMachine::registerState(const std::string &id, StateNode *state)
{
    statePool[id] = state;
}

void StateMachine::switchTo(const std::string &id)
{
    if (currentState)
    {
        currentState->exit();
    }

    currentState = statePool[id];

    if (currentState)
    {
        currentState->enter();
    }
}

void StateMachine::setEntry(const std::string &id)
{
    currentState = statePool[id];
}
