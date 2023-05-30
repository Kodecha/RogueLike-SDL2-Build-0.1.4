#include "Player.h"

#include <string>

using namespace std;

Player::Player(string name, int x, int y)
{
    this->name = name;
    this->x = x;
    this->y = y;
    this->hp = 100;
    this->maxHp = 100;
    this->mp = 100;
    this->maxMp = 100;
    this->atk = 10;
    this->def = 10;
    this->exp = 69;
    this->lvl = 1;
    this->gold = 0;
    this->maxGold = 100;
    this->maxExp = 100;
}

string Player::getName()
{
    return this->name;
}

int Player::getStat(string stat)
{
    if (stat == "x")
    {
        return this->x;
    }
    else if (stat == "y")
    {
        return this->y;
    }
    else if (stat == "hp")
    {
        return this->hp;
    }
    else if (stat == "maxHp")
    {
        return this->maxHp;
    }
    else if (stat == "mp")
    {
        return this->mp;
    }
    else if (stat == "maxMp")
    {
        return this->maxMp;
    }
    else if (stat == "atk")
    {
        return this->atk;
    }
    else if (stat == "def")
    {
        return this->def;
    }
    else if (stat == "exp")
    {
        return this->exp;
    }
    else if (stat == "lvl")
    {
        return this->lvl;
    }
    else if (stat == "gold")
    {
        return this->gold;
    }
    else if (stat == "maxGold")
    {
        return this->maxGold;
    }
    else if (stat == "maxExp")
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
    if (stat == "x")
    {
        this->x = value;
    }
    else if (stat == "y")
    {
        this->y = value;
    }
    else if (stat == "hp")
    {
        this->hp = value;
    }
    else if (stat == "maxHp")
    {
        this->maxHp = value;
    }
    else if (stat == "mp")
    {
        this->mp = value;
    }
    else if (stat == "maxMp")
    {
        this->maxMp = value;
    }
    else if (stat == "atk")
    {
        this->atk = value;
    }
    else if (stat == "def")
    {
        this->def = value;
    }
    else if (stat == "exp")
    {
        this->exp = value;
    }
    else if (stat == "lvl")
    {
        this->lvl = value;
    }
    else if (stat == "gold")
    {
        this->gold = value;
    }
    else if (stat == "maxGold")
    {
        this->maxGold = value;
    }
    else if (stat == "maxExp")
    {
        this->maxExp = value;
    }
}

void Player::addStat(string stat, int value)
{
    if (stat == "x")
    {
        this->x += value;
    }
    else if (stat == "y")
    {
        this->y += value;
    }
    else if (stat == "hp")
    {
        this->hp += value;
    }
    else if (stat == "maxHp")
    {
        this->maxHp += value;
    }
    else if (stat == "mp")
    {
        this->mp += value;
    }
    else if (stat == "maxMp")
    {
        this->maxMp += value;
    }
    else if (stat == "atk")
    {
        this->atk += value;
    }
    else if (stat == "def")
    {
        this->def += value;
    }
    else if (stat == "exp")
    {
        this->exp += value;
    }
    else if (stat == "lvl")
    {
        this->lvl += value;
    }
    else if (stat == "gold")
    {
        this->gold += value;
    }
    else if (stat == "maxGold")
    {
        this->maxGold += value;
    }
    else if (stat == "maxExp")
    {
        this->maxExp += value;
    }
}   

void Player::subStat(string stat, int value)
{
    if (stat == "x")
    {
        this->x -= value;
    }
    else if (stat == "y")
    {
        this->y -= value;
    }
    else if (stat == "hp")
    {
        this->hp -= value;
    }
    else if (stat == "maxHp")
    {
        this->maxHp -= value;
    }
    else if (stat == "mp")
    {
        this->mp -= value;
    }
    else if (stat == "maxMp")
    {
        this->maxMp -= value;
    }
    else if (stat == "atk")
    {
        this->atk -= value;
    }
    else if (stat == "def")
    {
        this->def -= value;
    }
    else if (stat == "exp")
    {
        this->exp -= value;
    }
    else if (stat == "lvl")
    {
        this->lvl -= value;
    }
    else if (stat == "gold")
    {
        this->gold -= value;
    }
    else if (stat == "maxGold")
    {
        this->maxGold -= value;
    }
    else if (stat == "maxExp")
    {
        this->maxExp -= value;
    }
}  




