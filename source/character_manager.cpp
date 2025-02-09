#include "player.h"
#include "enemy.h"
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
    enemy->update(delta);
    player->update(delta);
    timerLog.update(delta);
}

void CharacterManager::render()
{
    enemy->render();
    BulletTimeManager::Instance()->postProcess();
    player->render();
}

CharacterManager::CharacterManager()
{
    this->enemy = new Enemy();
    this->player = new Player();

    timerLog.setWaitTime(0.1f);
    timerLog.setOneShot(false);
    timerLog.setTimeOut([&]()
                        {
                            player->log();
                            enemy->log(); });
}

CharacterManager::~CharacterManager()
{
    delete player;
    delete enemy;
}