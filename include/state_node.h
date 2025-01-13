#ifndef _STATE_NODE_H_
#define _STATE_NODE_H_

class StateNode
{

public:
    StateNode() = default;
    ~StateNode() = default;

	virtual void enter() {}
    virtual void update(float delta) {}
    virtual void exit(){}
};

#endif // _STATE_NODE_H_