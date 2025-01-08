#include "resources_manager.h"
#include "util.h"
#include <iostream>
struct imgResInfo
{
    std::string id;
    LPCTSTR path;
};

struct atlasResInfo
{
    std::string id;
    LPCTSTR path;
    int frameNum = 0;
};

static const std::vector<imgResInfo> imgInfoList = {
    {_T("background"), _T(R"(assest\background.png)")},
    {_T("uiHeart"), _T(R"(assest\ui_heart.png)")},

    {_T("palyerAttackRigth"), _T(R"(assest\palyer\attack.png)")},
    {_T("palyerDeadRight"), _T(R"(assest\player\dead.png)")},
    {_T("playerFallRight"), _T(R"(assest\player\fall.png)")},
    {_T("playerIdleRight"), _T(R"(assest\player\idle.png)")},
    {_T("playerJumpRight"), _T(R"(assest\player\jump.png)")},
    {_T("playerRunRight"), _T(R"(assest\player\run.png)")},
    {_T("playerRollRight"), _T(R"(assest\player\roll.png)")},

    {_T("palyerVfxAttackDown"), _T(R"(assest\palyer\vfx_attack_down.png)")},
    {_T("palyerVfxAttackLeft"), _T(R"(assest\palyer\vfx_attack_left.png)")},
    {_T("palyerVfxAttackRight"), _T(R"(assest\palyer\vfx_attack_right.png)")},
    {_T("palyerVfxAttackUp"), _T(R"(assest\palyer\vfx_attack_up.png)")},
    {_T("palyerVfxJump"), _T(R"(assest\palyer\vfx_jump.png)")},
    {_T("palyerVfxLand"), _T(R"(assest\palyer\vfx_land.png)")},
};

static const std::vector<atlasResInfo> atlasInfoList = {
    {_T("barbBreak"), _T(R"(assest\enemy\barb_break\%d.png)"), 3},
    {_T("barbLoose"), _T(R"(assest\enemy\barb_loose\%d.png)"), 5},
    {_T("silk"), _T(R"(assest\enemy\silk\%d.png)"), 9},
    {_T("swordLeft"), _T(R"(assest\enemy\sword\%d.png)"), 3},

    {_T("enemyAimLeft"), _T(R"(assest\enemy\aim\%d.png)"), 9},
    {_T("enemyDashInAir"), _T(R"(assest\enemy\dash_in_air\%d.png)"), 2},
    {_T("enemyDashOnFloor"), _T(R"(assest\enemy\dash_on_floor\%d.png)"), 2},
    {_T("enemyFallLeft"), _T(R"(assest\enemy\fall\%d.png)"), 4},
    {_T("enemyIdleLeft"), _T(R"(assest\enemy\idle\%d.png)"), 6},
    {_T("enemyjumpLeft"), _T(R"(assest\enemy\jump\%d.png)"), 8},
    {_T("enemyRunLeft"), _T(R"(assest\enemy\run\%d.png)"), 8},
    {_T("enemySquatLeft"), _T(R"(assest\enemy\squat\%d.png)"), 10},
    {_T("enemyThrowBarbLeft"), _T(R"(assest\enemy\throw_barb\%d.png)"), 8},
    {_T("enemyThrowSilkLeft"), _T(R"(assest\enemy\throw_silk\%d.png)"), 17},
    {_T("enemyThrowSwordLeft"), _T(R"(assest\enemy\throw_sword\%d.png)"), 16},

    {_T("enemyVfxDashInAirLeft"), _T(R"(assest\enemy\vfx_dash_in_air\%d.png)"), 5},
    {_T("enemyVfxDashOnFloorLeft"), _T(R"(assest\enemy\vfx_dash_on_floor\%d.png)"), 6},
};

static inline bool checkImageValid(IMAGE *img)
{
    return img->getwidth() > 0 && img->getheight() > 0;
}

void ResourcesManager::load()
{
    for (auto &info : imgInfoList)
    {
        IMAGE *img = new IMAGE();
        loadimage(img, info.path);
        if (!checkImageValid(img))
        {
            throw info.path;
        }
        imagePool[info.id] = img;
    }

    for (auto &info : atlasInfoList)
    {
        Atlas *atlas = new Atlas();
        atlas->load(info.path, info.frameNum);
        for (int i = 0; i < atlas->getSize(); i++)
        {
            IMAGE *img = atlas->getImage(i);
            if (!checkImageValid(img))
            {
                throw info.path;
            }
        }
        atlasPool[info.id] = atlas;
    }

    flipImage(_T("palyerAttackRigth"), _T("palyerAttackLeft"), 5);
    flipImage(_T("palyerDeadRight"), _T("palyerDeadLeft"), 6);
    flipImage(_T("playerFallRight"), _T("playerFallLeft"), 5);
    flipImage(_T("playerIdleRight"), _T("palyerIdleLeft"), 5);
    flipImage(_T("playerJumpRight"), _T("playerJumpLeft"), 5);
    flipImage(_T("playerRunRight"), _T("playerRunLeft"), 10);
    flipImage(_T("playerRollRight"), _T("playerRollLeft"), 7);
    flipAtlas(_T("swordLeft"), _T("swordRight"));
    flipAtlas(_T("enemyAimLeft"), _T("enemyAimRight"));
    flipAtlas(_T("enemyDashInAir"), _T("enemyDashInAirRight"));
    flipAtlas(_T("enemyDashOnFloor"), _T("enemyDashOnFloorRight"));
    flipAtlas(_T("enemyFallLeft"), _T("enemyFallRight"));
    flipAtlas(_T("enemyIdleLeft"), _T("enemyIdleRight"));
    flipAtlas(_T("enemyjumpLeft"), _T("enemyjumpRight"));
    flipAtlas(_T("enemyRunLeft"), _T("enemyRunRight"));
    flipAtlas(_T("enemySquatLeft"), _T("enemySquatRight"));
    flipAtlas(_T("enemyThrowBarbLeft"), _T("enemyThrowBarbRight"));
    flipAtlas(_T("enemyThrowSilkLeft"), _T("enemyThrowSilkRight"));
    flipAtlas(_T("enemyThrowSwordLeft"), _T("enemyThrowSwordRight"));
    flipAtlas(_T("enemyVfxDashInAirLeft"), _T("enemyVfxDashInAirRight"));
    flipAtlas(_T("enemyVfxDashOnFloorLeft"), _T("enemyVfxDashOnFloorRight"));

    loadAudio(_T(R"(assest\audio\bgm.mp3)"), _T("bgm"));
    loadAudio(_T(R"(assest\audio\barb_break.mp3)"), _T("barbBreak"));
    loadAudio(_T(R"(assest\audio\bullet_time.mp3)"), _T("bulletTime"));

    loadAudio(_T(R"(assest\audio\enemy_dash.mp3)"), _T("enemyDash"));
    loadAudio(_T(R"(assest\audio\enemy_run.mp3)"), _T("enemyRun"));
    loadAudio(_T(R"(assest\audio\enemy_hurt_1.mp3)"), _T("enemyHurt1"));
    loadAudio(_T(R"(assest\audio\enemy_hurt_2.mp3)"), _T("enemyHurt2"));
    loadAudio(_T(R"(assest\audio\enemy_hurt_3.mp3)"), _T("enemyHurt3"));
    loadAudio(_T(R"(assest\audio\enemy_throw_barb.mp3)"), _T("enemyThrowBarb"));
    loadAudio(_T(R"(assest\audio\enemy_throw_silk.mp3)"), _T("enemyThrowSilk"));
    loadAudio(_T(R"(assest\audio\enemy_throw_sword.mp3)"), _T("enemyThrowSword"));

    loadAudio(_T(R"(assest\audio\player_attack_1.mp3)"), _T("playerAttack1"));
    loadAudio(_T(R"(assest\audio\player_attack_2.mp3)"), _T("playerAttack2"));
    loadAudio(_T(R"(assest\audio\player_attack_3.mp3)"), _T("playerAttack3"));
    loadAudio(_T(R"(assest\audio\player_dead.mp3)"), _T("playerDead"));
    loadAudio(_T(R"(assest\audio\player_hurt.mp3)"), _T("playerHurt"));
    loadAudio(_T(R"(assest\audio\player_jump.mp3)"), _T("playerJump"));
    loadAudio(_T(R"(assest\audio\player_land.mp3)"), _T("playerLand"));
    loadAudio(_T(R"(assest\audio\player_roll.mp3)"), _T("playerRol_T("));
    loadAudio(_T(R"(assest\audio\player_run.mp3)"), _T("playerRun"));
}

Atlas *ResourcesManager::findAtlas(const std::string &id) const
{
    const auto &it = atlasPool.find(id);
    if (it == atlasPool.end())
    {
        return nullptr;
    }
    return it->second;
}

IMAGE *ResourcesManager::findImage(const std::string &name) const
{
    const auto &it = imagePool.find(name);
    if (it == imagePool.end())
    {
        return nullptr;
    }
    return it->second;
}

void ResourcesManager::flipImage(IMAGE *srcImg, IMAGE *dstImg, int numH)
{
    int w = srcImg->getwidth();
    int h = srcImg->getheight();
    int wFrame = w / numH;
    Resize(dstImg, w, h);
    DWORD *srcBuffer = GetImageBuffer(srcImg);
    DWORD *dstBuffer = GetImageBuffer(dstImg);
    for (int i = 0; i < numH; i++)
    {
        int xLeft = i * wFrame;
        int xRight = (i + 1) * wFrame;
        for (int y = 0; y < h; y++)
        {
            for (int x = xLeft; x < xRight; x++)
            {
                int idxSrc = y * w + x;
                int idxDst = y * w + xRight - (x - xLeft);
                dstBuffer[idxDst] = srcBuffer[idxSrc];
            }
        }
    }
}

void ResourcesManager::flipImage(const std::string &srcId, const std::string &dstId, int numH)
{
    IMAGE *srcImg = findImage(srcId);
    IMAGE *dstImg = new IMAGE();
    flipImage(srcImg, dstImg, numH);
    imagePool[dstId] = dstImg;
}

void ResourcesManager::flipAtlas(const std::string &srcId, const std::string &dstId)
{
    Atlas *srcAtlas = findAtlas(srcId);
    Atlas *dstAtlas = new Atlas();
    for (int i = 0; i < srcAtlas->getSize(); i++)
    {
        IMAGE dstImg;
        flipImage(srcAtlas->getImage(i), &dstImg);
        dstAtlas->addImage(dstImg);
    }
    atlasPool[dstId] = dstAtlas;
}

ResourcesManager *ResourcesManager::manager = nullptr;

ResourcesManager *ResourcesManager::Instance()
{
    if (!manager)
    {
        manager = new ResourcesManager();
    }
    return manager;
}

ResourcesManager::ResourcesManager() = default;

ResourcesManager::~ResourcesManager() = default;