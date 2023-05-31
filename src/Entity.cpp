#include "Entity.h"
#include <string>

using namespace std;


Entity::Entity(string type, string name, int x, int y)
{
    this->type = type;
    this->name = name;
    this->x = x;
    this->y = y;
}

string Entity::getType()
{
    return this->type;
}

string Entity::getName()
{
    return this->name;
}

int Entity::getX()
{
    return this->x;
}

int Entity::getY()
{
    return this->y;
}

void Entity::setX(int x)
{
    this->x = x;
}

void Entity::setY(int y)
{
    this->y = y;
}

void Entity::addX(int x)
{
    this->x += x;
}

void Entity::addY(int y)
{
    this->y += y;
}

