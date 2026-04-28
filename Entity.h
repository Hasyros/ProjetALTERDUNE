#ifndef ENTITY_H
#define ENTITY_H

#include <string>

// Classe mere abstraite pour tout ce qui a des HP et qui peut taper
// (le joueur ET les monstres heritent de ca)
class Entity {
protected:
    std::string name;
    int hp;
    int hpMax;
    int atk;
    int def;

public:
    Entity(const std::string& nom, int pv, int attaque, int defense);
    virtual ~Entity();

    // getters
    std::string getName() const;
    int getHp() const;
    int getHpMax() const;
    int getAtk() const;
    int getDef() const;

    // setters / actions
    void setHp(int newHp);
    bool isAlive() const;
    void takeDamage(int dmg);

    // virtuelle pure -> rend la classe abstraite
    virtual void display() const = 0;
};

#endif
