#ifndef ENTITY_H
#define ENTITY_H

#include <string>

using namespace std;

class Entity
{
    private:
        string name;
        int x;
        int y;
    public:
        Entity(string name, int x, int y);

        string getName();
        int getX();
        int getY();

        void setName(string name);
        void setX(int x);
        void setY(int y);
};

#endif