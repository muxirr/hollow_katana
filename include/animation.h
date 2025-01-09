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
    bool setOnFinished(std::function<void()> onFinished)
    {
        this->onFinished = onFinished;
    }
    void addFrame(IMAGE *img, int num_h)
    {
        int w = img->getwidth();
        int h = img->getheight();
        int w_frame = w / num_h;
        for (int i = 0; i < num_h; i++)
        {
            Rect rect_src;
            rect_src.x = i * w_frame;
            rect_src.y = 0;
            rect_src.w = w_frame;
            rect_src.h = h;
            frameList.emplace_back(img, rect_src);
        }
    }
    void addFrame(Atlas &atlas)
    {
        for (int i = 0; i < atlas.getSize(); i++)
        {
            IMAGE *img = atlas.getImage(i);
            Rect rect_src;
            rect_src.x = 0;
            rect_src.y = 0;
            rect_src.w = img->getwidth();
            rect_src.h = img->getheight();
            frameList.emplace_back(img, rect_src);
        }
    }

    void onUpdate(float delta)
    {
        timer.onUpdate(delta);
    }

    void onRender()
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