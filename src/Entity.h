#ifndef ENTITY_H
#define ENTITY_H

#include <string>

using namespace std;

class Entity
{
    private:
        string type;
        string name;
        int x;
        int y;  
    
    public:
        Entity();
        Entity(string type, string name, int x, int y);
        string getType();
        string getName();
        int getX();
        int getY();
        void setX(int x);
        void setY(int y);
        void addX(int x);
        void addY(int y);
};

#endif // ENTITY_H

