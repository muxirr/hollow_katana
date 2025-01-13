#include "player.h"
#include "bullet_time_manager.h"
#include "character_manager.h"

CharacterManager *CharacterManager::manager = nullptr;

CharacterManager *CharacterManager::Instance()
{
    if (manager == nullptr)
    {
        manager = new CharacterManager();
    }
    return manager;
}

void CharacterManager::input(const ExMessage &msg)
{
    player->input(msg);
}

void CharacterManager::update(float delta)
{
    player->update(delta);

    timerLog.update(delta);
}

void CharacterManager::render()
{
    BulletTimeManager::Instance()->postProcess();
    player->render();
}

void CharacterManager::log()
{
    player->log();
}

CharacterManager::CharacterManager()
{
    this->player = new Player();

    timerLog.setWaitTime(1.0f);
    timerLog.setOneShot(false);
    timerLog.setTimeOut([&]()
                        { player->log(); });
}

CharacterManager::~CharacterManager()
{
    delete player;
    delete enemy;
}