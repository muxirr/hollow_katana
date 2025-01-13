#include "util.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "collision_manager.h"
#include "bullet_time_manager.h"
#include "test_defines.h"

#include <windows.h>
#include <graphics.h>
#include <chrono>
#include <thread>

const int FPS = 144;
const long long NANOSECONDS_PER_SECOND = 1000000000LL;

static void drawBackground()
{
    static IMAGE *imgBackground = ResourcesManager::Instance()->findImage(_T("background"));
    static Rect rectDst = {(getwidth() - imgBackground->getwidth()) / 2, (getheight() - imgBackground->getheight()) / 2, imgBackground->getwidth(), imgBackground->getheight()};
    putimageEx(imgBackground, &rectDst);
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // 初始化窗口1
    using namespace std::chrono;
#ifdef DEBUG
    HWND hwnd = initgraph(1280, 720, SHOWCONSOLE);
#else
    HWND hwnd = initgraph(1280, 720);
#endif
    SetWindowText(hwnd, _T("Hollow Katana"));
    setbkcolor(RGB(0, 0, 0));

    // 加载资源1
    try
    {
        ResourcesManager::Instance()->load();
    }
    catch (const LPCSTR id)
    {
        TCHAR errMsg[512];
        wsprintf(errMsg, _T("Unable to load: %hs"), id);
        MessageBox(hwnd, errMsg, _T("Resource loading failure"), MB_OK | MB_ICONERROR);
        return -1;
    }

    const nanoseconds frameDuration(NANOSECONDS_PER_SECOND / FPS); // 144 FPS
    steady_clock::time_point lastTime = steady_clock::now();
    ExMessage msg;
    bool isQuit = false;

    // 游戏主循环1
    BeginBatchDraw();
    while (!isQuit)
    {
        // 处理消息1
        while (peekmessage(&msg))
        {
            CharacterManager::Instance()->input(msg);
        }

        steady_clock::time_point frameStart = steady_clock::now();
        duration<double> delta = duration<float>(frameStart - lastTime);

        // 处理更新1
        float scaledTime = BulletTimeManager::Instance()->update(delta.count());
        CharacterManager::Instance()->update(scaledTime);
        CollisionManager::Instance()->processCollide();

        cleardevice();

        // 处理绘图1
        drawBackground();
        CharacterManager::Instance()->render();
        CollisionManager::Instance()->onDebugrender();

#ifdef SHOW_FPS
        double fps = 1.0 / delta.count();
        TCHAR fpsStr[32];
        sprintf(fpsStr, _T("FPS: %.2f"), fps);
        outtextxy(10, 10, fpsStr);
#endif
        FlushBatchDraw();

        // 更新时间1
        lastTime = frameStart;

        // 计算帧耗时1
        nanoseconds sleepDuration = duration_cast<nanoseconds>(steady_clock::now() - frameStart);

        // 如果帧耗时大于0
        if (sleepDuration > nanoseconds(0))
        {
            // 休眠1
            std::this_thread::sleep_for(frameDuration - sleepDuration);
        }
    }

    EndBatchDraw();

    return 0;
}