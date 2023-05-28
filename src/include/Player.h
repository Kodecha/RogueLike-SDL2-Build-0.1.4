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
        int getX();
        int getY();
        void setName(string name);
        void setX(int x);
        void setY(int y);
        int getHp();
        int getMaxHp();
        int getMp();
        int getMaxMp();
        int getAtk();
        int getDef();
        int getExp();
        int getLvl();
        int getGold();
        int getMaxGold();
        int getMaxExp();
        void setHp(int hp);
        void setMaxHp(int maxHp);
        void setMp(int mp);
        void setMaxMp(int maxMp);
        void setAtk(int atk);
        void setDef(int def);
        void setExp(int exp);
        void setLvl(int lvl);
        void setGold(int gold);
        void setMaxGold(int maxGold);
        void setMaxExp(int maxExp);
};

#endif
