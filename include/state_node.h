#ifndef _STATE_NODE_H_
#define _STATE_NODE_H_

class StateNode {

public:
    StateNode() = default;
    ~StateNode() = default;

    virtual void enter() = 0;
    virtual void update(float delta) = 0;
    virtual void exit() = 0;

};

#endif // _STATE_NODE_H_