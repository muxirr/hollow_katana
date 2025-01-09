#ifndef _COLLISION_BOX_H_
#define _COLLISION_BOX_H_

#include "vector2.h"
#include "collision_layer.h"

#include <functional>

class CollisionManager;

class CollisionBox
{
    friend class CollisionManager;

public:
    void setSize(const Vector2 &size)
    {
        this->size = size;
    }

    void setPosition(const Vector2 &position)
    {
        this->position = position;
    }

    void setEnabled(bool enabled)
    {
        this->enabled = enabled;
    }

    void setOnCollide(std::function<void()> onCollide)
    {
        this->onCollide = onCollide;
    }

    void setLayerSrc(CollisionLayer layerSrc)
    {
        this->layerSrc = layerSrc;
    }

    void setLayerDst(CollisionLayer layerDst)
    {
        this->layerDst = layerDst;
    }

    const Vector2 &getSize() const
    {
        return size;
    }

private:
    Vector2 size;
    Vector2 position;
    bool enabled = true;
    std::function<void()> onCollide;
    CollisionLayer layerSrc = CollisionLayer::None;
    CollisionLayer layerDst = CollisionLayer::None;

private:
    CollisionBox() = default;
    ~CollisionBox() = default;
};

#endif // _COLLISION_BOX_H_