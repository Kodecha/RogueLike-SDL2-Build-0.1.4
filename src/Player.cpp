#include "include/Player.h"

Player::Player(string name, int x, int y)
{
    this->name = name;
    this->x = x;
    this->y = y;
}

string Player::getName()
{
    return name;
}

int Player::getX()
{
    return x;
}

int Player::getY()
{
    return y;
}

void Player::setName(string name)
{
    this->name = name;
}

void Player::setX(int x)
{
    this->x = x;
}

void Player::setY(int y)
{
    this->y = y;
}