#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include "atlas.h"
#include <string>
#include <graphics.h>
#include <unordered_map>

class ResourcesManager
{
public:
    static ResourcesManager *Instance();

    void load();

    Atlas *findAtlas(const std::string &name) const;
    IMAGE *findImage(const std::string &name) const;

private:
    static ResourcesManager *manager;

    std::unordered_map<std::string, Atlas *> atlasPool;
    std::unordered_map<std::string, IMAGE *> imagePool;

private:
    ResourcesManager();
    ~ResourcesManager();

    void flipImage(IMAGE *srcImg, IMAGE *dstImg, int numH = 1);
    void flipImage(const std::string &srcId, const std::string &dstId, int numH = 1);
    void flipAtlas(const std::string &srcId, const std::string &dstId);
};

#endif // _RESOURCES_MANAGER_H_