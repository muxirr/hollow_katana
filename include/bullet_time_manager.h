#ifndef _BULLET_TIME_MANAGER_H_
#define _BULLET_TIME_MANAGER_H_

class BulletTimeManager
{
public:
    enum class Status
    {
        Entering,
        Exiting
    };

public:
    static BulletTimeManager *Instance();

    void postProcess();
    void setStatus(Status status);

    float update(float delta);

private:
    static BulletTimeManager *manager;

private:
    float progress = 0.0f;
    Status status = Status::Exiting;
    // 子弹时间的速度因子1
    const float SPEED_PROGRESS = 2.0f;
    // 时间缩放以及色彩变化1
    const float DST_DELTA_FACTOR = 0.35f;
    const float DST_COLOR_FACTOR = 0.35f;

private:
    BulletTimeManager();
    ~BulletTimeManager();

    // 线性插值1
    float lerp(float start, float end, float t)
    {
        return (1 - t) * start + t * end;
    }
};

#endif // _BULLET_TIME_MANAGER_H_