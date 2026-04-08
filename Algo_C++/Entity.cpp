#include "Entity.h"
#include <iostream>

Entity::Entity(const std::string& name, int hp, int atk, int def)
    : name(name), hp(hp), hpMax(hp), atk(atk), def(def) {}

Entity::~Entity() {}

std::string Entity::getName() const { return name; }
int Entity::getHp() const { return hp; }
int Entity::getHpMax() const { return hpMax; }
int Entity::getAtk() const { return atk; }
int Entity::getDef() const { return def; }

void Entity::setHp(int newHp) {
    hp = newHp;
    if (hp < 0) hp = 0;
    if (hp > hpMax) hp = hpMax;
}

bool Entity::isAlive() const {
    return hp > 0;
}

void Entity::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}
