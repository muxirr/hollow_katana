#ifndef _ANIMATION_H
#define _ANIMATION_H

#include "util.h"
#include "timer.h"
#include "atlas.h"
#include "vector2.h"

#include <vector>
#include <functional>

class Animation
{
public:
    enum class AnchorMode
    {
        Centered,
        BottomCentered,
    };
    Animation()
    {
        timer.setOneShot(false);
        timer.setTimeOut([&]()
                         {  idxFrame++;
                            if(idxFrame >= frameList.size()){
                                idxFrame = isLoop?0:frameList.size()-1;
                                if(!isLoop && onFinished)
                                 onFinished();                              
                            } });
    }
    ~Animation() = default;

    void reset()
    {
        timer.restart();
        idxFrame = 0;
    }
    void setPosition(const Vector2 &position)
    {
        this->position = position;
    }
    void setLoop(bool isLoop)
    {
        this->isLoop = isLoop;
    }
    void setInterval(float interval)
    {
        timer.setWaitTime(interval);
    }
    /*
    param: Centered, BottomCentered
    */
    void setAnchorMode(AnchorMode anchormode)
    {
        this->anchormode = anchormode;
    }
    void setOnFinished(std::function<void()> onFinished)
    {
        this->onFinished = onFinished;
    }
    void addFrame(IMAGE *img, int numH)
    {
        int w = img->getwidth();
        int h = img->getheight();
        int wFrame = w / numH;
        for (int i = 0; i < numH; i++)
        {
            Rect rectSrc;
            rectSrc.x = i * wFrame;
            rectSrc.y = 0;
            rectSrc.w = wFrame;
            rectSrc.h = h;
            frameList.emplace_back(img, rectSrc);
        }
    }
    void addFrame(Atlas &atlas)
    {
        for (int i = 0; i < atlas.getSize(); i++)
        {
            IMAGE *img = atlas.getImage(i);
            Rect rectSrc;
            rectSrc.x = 0;
            rectSrc.y = 0;
            rectSrc.w = img->getwidth();
            rectSrc.h = img->getheight();
            frameList.emplace_back(img, rectSrc);
        }
    }

    void update(float delta)
    {
        timer.update(delta);
    }

    void render()
    {
        const Frame &frame = frameList[idxFrame];

        Rect rectDst;
        rectDst.x = (int)position.x - frame.rectSrc.w / 2;
        rectDst.y = (anchormode == AnchorMode::Centered) ? (int)position.y - frame.rectSrc.h / 2 : (int)position.y - frame.rectSrc.h;
        rectDst.w = frame.rectSrc.w;
        rectDst.h = frame.rectSrc.h;
        putimageEx(frame.img, &rectDst, &frame.rectSrc);
    }

private:
    struct Frame
    {
        Rect rectSrc;
        IMAGE *img = nullptr;

        Frame() = default;
        Frame(IMAGE *img, const Rect &rectSrc) : img(img), rectSrc(rectSrc)
        {
        }
        ~Frame() = default;
    };

private:
    Timer timer;
    Vector2 position;
    bool isLoop = false;
    size_t idxFrame = 0;
    std::vector<Frame> frameList;
    std::function<void()> onFinished;
    AnchorMode anchormode = AnchorMode::Centered;
};

#endif // _ANIMATION_H