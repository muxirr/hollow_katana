#include "util.h"
#include "resources_manager.h"

#include <graphics.h>
#include <windows.h>
#include <chrono>
#include <thread>

const int FPS = 144;
const long long NANOSECONDS_PER_SECOND = 1000000000LL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    using namespace std::chrono;
    HWND hwnd = initgraph(1280, 720, EW_SHOWCONSOLE);
    SetWindowText(hwnd, _T("Hollow Katana"));

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

    BeginBatchDraw();
    while (!isQuit)
    {
        while (peekmessage(&msg))
        {
            // 处理消息
        }
        steady_clock::time_point frameStart = steady_clock::now();
        duration<double> delta = duration<float>(frameStart - lastTime);
        // 处理更新
        setbkcolor(RGB(0, 0, 0));
        cleardevice();
        // 处理绘图
        putimage(0, 0, ResourcesManager::Instance()->findImage(_T("background")));
        FlushBatchDraw();
        // 更新时间
        lastTime = frameStart;
        // 计算帧耗时
        nanoseconds sleepDuration = duration_cast<nanoseconds>(steady_clock::now() - frameStart);
        if (sleepDuration > nanoseconds(0)) // 如果帧耗时大于0
        {
            // 休眠
            std::this_thread::sleep_for(frameDuration - sleepDuration);
        }
    }

    EndBatchDraw();

    return 0;
}