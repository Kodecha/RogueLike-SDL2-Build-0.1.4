#include "Player.h"
#include "Entity.h"
#include <string>

using namespace std;

Player::Player(string name, int x, int y) : Entity("player", name, x, y)
{
    this->hp = 10;
    this->maxHp = 10;
    this->mp = 15;
    this->maxMp = 20;
    this->atk = 1;
    this->def = 1;
    this->exp = 200;
    this->level = 1;
    this->maxExp = 500;
}

int Player::getStat(string stat)
{
    if(stat == "hp")
    {
        return this->hp;
    }
    else if(stat == "maxHp")
    {
        return this->maxHp;
    }
    else if(stat == "mp")
    {
        return this->mp;
    }
    else if(stat == "maxMp")
    {
        return this->maxMp;
    }
    else if(stat == "atk")
    {
        return this->atk;
    }
    else if(stat == "def")
    {
        return this->def;
    }
    else if(stat == "exp")
    {
        return this->exp;
    }
    else if(stat == "level")
    {
        return this->level;
    }
    else if(stat == "maxExp")
    {
        return this->maxExp;
    }
    else
    {
        return 0;
    }
}

void Player::setStat(string stat, int value)
{
    if(stat == "hp")
    {
        this->hp = value;
    }
    else if(stat == "maxHp")
    {
        this->maxHp = value;
    }
    else if(stat == "mp")
    {
        this->mp = value;
    }
    else if(stat == "maxMp")
    {
        this->maxMp = value;
    }
    else if(stat == "atk")
    {
        this->atk = value;
    }
    else if(stat == "def")
    {
        this->def = value;
    }
    else if(stat == "exp")
    {
        this->exp = value;
    }
    else if(stat == "level")
    {
        this->level = value;
    }
    else if(stat == "maxExp")
    {
        this->maxExp = value;
    }
}

void Player::addStat(string stat, int value)
{
    if(stat == "hp")
    {
        this->hp += value;
    }
    else if(stat == "maxHp")
    {
        this->maxHp += value;
    }
    else if(stat == "mp")
    {
        this->mp += value;
    }
    else if(stat == "maxMp")
    {
        this->maxMp += value;
    }
    else if(stat == "atk")
    {
        this->atk += value;
    }
    else if(stat == "def")
    {
        this->def += value;
    }
    else if(stat == "exp")
    {
        this->exp += value;
    }
    else if(stat == "level")
    {
        this->level += value;
    }
    else if(stat == "maxExp")
    {
        this->maxExp += value;
    }
}

void Player::subStat(string stat, int value)
{
    if(stat == "hp")
    {
        this->hp -= value;
    }
    else if(stat == "maxHp")
    {
        this->maxHp -= value;
    }
    else if(stat == "mp")
    {
        this->mp -= value;
    }
    else if(stat == "maxMp")
    {
        this->maxMp -= value;
    }
    else if(stat == "atk")
    {
        this->atk -= value;
    }
    else if(stat == "def")
    {
        this->def -= value;
    }
    else if(stat == "exp")
    {
        this->exp -= value;
    }
    else if(stat == "level")
    {
        this->level -= value;
    }
    else if(stat == "maxExp")
    {
        this->maxExp -= value;
    }
}
