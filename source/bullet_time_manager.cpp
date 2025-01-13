#include "bullet_time_manager.h"

#include <graphics.h>

BulletTimeManager *BulletTimeManager::manager = nullptr;

BulletTimeManager *BulletTimeManager::Instance()
{
    if (manager == nullptr)
    {
        manager = new BulletTimeManager();
    }

    return manager;
}

void BulletTimeManager::postProcess()
{
    DWORD *buffer = GetImageBuffer();
    int w = getwidth();
    int h = getheight();

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            // 调整画面色彩1
            int idx = y * w + x;
            DWORD color = buffer[idx];
            BYTE r = GetBValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress);
            BYTE g = GetGValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress);
            BYTE b = GetRValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress);
            buffer[idx] = BGR(RGB(r, g, b)) | (((DWORD)(BYTE)(255)) << 24); // ff << 24
        }
    }
}

void BulletTimeManager::setStatus(Status status)
{
    this->status = status;
}

float BulletTimeManager::update(float delta)
{
    float deltaProgress = SPEED_PROGRESS * delta;

    progress += (status == Status::Entering ? deltaProgress : -deltaProgress); // 进入状态则增加，退出状态则减少1

    // 限制进度值在 0.0f ~ 1.0f 之间1
    if (progress < 0.0f)
    {
        progress = 0.0f;
    }
    else if (progress > 1.0f)
    {
        progress = 1.0f;
    }

    return delta * lerp(1.0f, DST_DELTA_FACTOR, progress);
}

BulletTimeManager::BulletTimeManager() = default;

BulletTimeManager::~BulletTimeManager() = default;