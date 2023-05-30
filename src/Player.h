#ifndef PLAYER_H
#define PLAYER_H

#include <string>

using namespace std;

class Player
{
    private:
        string name;
        int x;
        int y;
        int hp;
        int maxHp;
        int mp;
        int maxMp;
        int atk;
        int def;
        int exp;
        int lvl;
        int gold;
        int maxGold;
        int maxExp;
    public:
        Player(string name, int x, int y);
        string getName();
        int getStat(string stat);
        void setStat(string stat, int value);
        void addStat(string stat, int value);
        void subStat(string stat, int value);
};

#endif
