#ifndef _UTIL_H_
#define _UTIL_H_

#include <graphics.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

struct Rect
{
    int x, y;
    int w, h;
};

inline void putimageEx(IMAGE *img, const Rect *rectDst, const Rect *rectSrc = nullptr)
{
    static BLENDFUNCTION blendFunc = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

    AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(rectDst->x), (int)(rectDst->y), rectDst->w, rectDst->h, GetImageHDC(img), rectSrc ? rectSrc->x : 0, rectSrc ? rectSrc->y : 0, rectSrc ? rectSrc->w : img->getwidth(), rectSrc ? rectSrc->h : img->getheight(), blendFunc);
}

inline void loadAudio(LPCTSTR path, LPCTSTR id)
{
    static TCHAR strCmd[512];
    wsprintf(strCmd, _T("open %s alias %s"), path, id);
    mciSendString(strCmd, NULL, 0, NULL);
}

inline void playAudio(LPCTSTR id, bool isLoop = false)
{
    static TCHAR strCmd[512];
    wsprintf(strCmd, _T("play %s %s from 0"), id, isLoop ? _T("repeat") : _T(""));
    mciSendString(strCmd, NULL, 0, NULL);
}

inline void stopAudio(LPCTSTR id)
{
    static TCHAR strCmd[512];
    wsprintf(strCmd, _T("stop %s"), id);
    mciSendString(strCmd, NULL, 0, NULL);
}

inline int range_random(int min, int max)
{
    return min + rand() % (max - min + 1);
}

#endif