#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

    void restart()
    {
        this->passTime = 0;
        this->shotted = false;
    }

    void setWaitTime(float waitTime)
    {
        this->waitTime = waitTime;
    }

    void setOneShot(bool oneShot)
    {
        this->oneShot = oneShot;
    }

    void setTimeOut(std::function<void()> onTimeout)
    {
        this->onTimeout = onTimeout;
    }

    void pause()
    {
        this->paused = true;
    }

    void resume()
    {
        this->paused = false;
    }

    void update(float delta)
    {
        if (paused)
            return;
        this->passTime += delta;
        if (this->passTime >= this->waitTime)
        {
            bool canShot = (!oneShot || (oneShot && !shotted));
            this->shotted = true;
            if (canShot && onTimeout)
                this->onTimeout();
            this->passTime -= this->waitTime;
        }
    }

private:
    float passTime = 0;
    float waitTime = 0;
    bool paused = false;
    bool shotted = false;
    bool oneShot = false;
    std::function<void()> onTimeout;
};

#endif // _TIMER_H_