#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <string>

using namespace std;

class Player: public Entity
{
    private:
        int hp;
        int maxHp;
        int mp;
        int maxMp;
        int atk;
        int def;
        int exp;
        int level;
        int maxExp;
    public:
        Player(string name, int x, int y);
        int getStat(string stat);
        void setStat(string stat, int value);
        void addStat(string stat, int value);
        void subStat(string stat, int value);   
};

#endif
