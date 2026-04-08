#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
protected:
    std::string name;
    int hp;
    int hpMax;
    int atk;
    int def;

public:
    Entity(const std::string& name, int hp, int atk, int def);
    virtual ~Entity();

    std::string getName() const;
    int getHp() const;
    int getHpMax() const;
    int getAtk() const;
    int getDef() const;

    void setHp(int hp);
    bool isAlive() const;
    void takeDamage(int damage);

    virtual void display() const = 0;
};

#endif
