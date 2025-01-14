#include "util.h"
#include "resources_manager.h"
#include "character_manager.h"
#include "collision_manager.h"
#include "bullet_time_manager.h"

#include <windows.h>
#include <graphics.h>
#include <chrono>
#include <thread>
#include <fstream>

#define SHOW_FPS

int FPS = 144;
const long long NANOSECONDS_PER_SECOND = 1000000000LL;

void drawBackground();
void readConfig(HWND hwnd);
static void drawRemianHp();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
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

    playAudio(_T("bgm"), true);
    readConfig(hwnd);

    const nanoseconds frameDuration(NANOSECONDS_PER_SECOND / FPS);
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
        drawRemianHp();
        CharacterManager::Instance()->render();

#ifdef DEBUG
        CollisionManager::Instance()->debugRender();
#endif

#ifdef SHOW_FPS
        double fps = 1.0 / delta.count();
        TCHAR fpsStr[32];
        sprintf(fpsStr, _T("FPS: %.2f"), fps);
        outtextxy(10, 705, fpsStr);
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

static void drawBackground()
{
    static IMAGE *imgBackground = ResourcesManager::Instance()->findImage(_T("background"));
    static Rect rectDst = {(getwidth() - imgBackground->getwidth()) / 2, (getheight() - imgBackground->getheight()) / 2, imgBackground->getwidth(), imgBackground->getheight()};
    putimageEx(imgBackground, &rectDst);
}

void readConfig(HWND hwnd)
{
    std::ifstream ifs("./assest/config.ini");
    if (!ifs.is_open())
    {
        MessageBox(hwnd, _T("Unable to open assest/config.ini"), _T("Error"), MB_OK | MB_ICONERROR);
        std::exit(-1);
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::string key, value, split;
        std::istringstream iss(line);
        iss >> key >> split >> value;

        if (key == "FPS")
        {
            FPS = std::stoi(value);
        }
    }
}

static void drawRemianHp()
{
    static IMAGE *imgUiHeart = ResourcesManager::Instance()->findImage(_T("uiHeart"));
    static IMAGE *enemyIdle = ResourcesManager::Instance()->findAtlas(_T("enemyIdleLeft"))->getImage(0);
    static IMAGE *playerIdle = ResourcesManager::Instance()->findImage(_T("playerIdleRight"));

    static int w = playerIdle->getwidth() / 5;
    static float playerScale = playerIdle->getheight() / static_cast<float>(w);
    static float enemyScale = static_cast<float>(enemyIdle->getheight()) / enemyIdle->getwidth();

    static Rect rectDst = {0, 10, imgUiHeart->getwidth(), imgUiHeart->getheight()};
    static Rect rectPlayerDst = {0, 5, imgUiHeart->getheight() / playerScale, imgUiHeart->getheight()};
    static Rect rectEnemyDst = {1240, 10, imgUiHeart->getheight() / enemyScale, imgUiHeart->getheight()};
    static Rect rectPlayerSrc = {0, 0, w, playerIdle->getheight()};
    static Rect rectEnemySrc = {0, 0, enemyIdle->getwidth(), enemyIdle->getheight()};

    putimageEx(playerIdle, &rectPlayerDst, &rectPlayerSrc);
    putimageEx(enemyIdle, &rectEnemyDst, &rectEnemySrc);

    for (int i = 0; i < CharacterManager::Instance()->getPlayer()->getHp(); i++)
    {
        rectDst.x = 40 + i * 40;
        putimageEx(imgUiHeart, &rectDst);
    }
    for (int i = 0; i < CharacterManager::Instance()->getEnemy()->getHp(); i++)
    {
        rectDst.x = 1200 - i * 40;
        putimageEx(imgUiHeart, &rectDst);
    }
}