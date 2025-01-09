#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "state_node.h"

#include <string>
#include <graphics.h>
#include <unordered_map>

class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    void update(float delta);
    void registerState(const std::string &id, StateNode *state);
    void switchTo(const std::string &id);
    void setEntry(const std::string &id);

private:
    bool needInit = true;
    StateNode *currentState = nullptr;
    std::unordered_map<std::string, StateNode *> statePool;
};

#endif // _STATE_MACHINE_H_