#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "character.h"

class CharacterManager
{
public:
    static CharacterManager *Instance();

    Character *getEnemy()
    {
        return enemy;
    }

    Character *getPlayer()
    {
        return player;
    }

    void input(const ExMessage &msg);
    void update(float delta);
    void render();

private:
    static CharacterManager *manager;

    Character *player = nullptr;
    Character *enemy = nullptr;

    Timer timerLog;

private:
    CharacterManager();
    ~CharacterManager();
};

#endif // _CHARACTER_MANAGER_H_