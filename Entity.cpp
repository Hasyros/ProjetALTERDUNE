#include "Entity.h"

using namespace std;

Entity::Entity(const string& nom, int pv, int attaque, int defense)
    : name(nom), hp(pv), hpMax(pv), atk(attaque), def(defense) {}

Entity::~Entity() {}

string Entity::getName() const { return name; }
int Entity::getHp()    const { return hp; }
int Entity::getHpMax() const { return hpMax; }
int Entity::getAtk()   const { return atk; }
int Entity::getDef()   const { return def; }

void Entity::setHp(int newHp) {
    hp = newHp;
    // on borne pour eviter les valeurs n'importe quoi
    if (hp < 0) hp = 0;
    if (hp > hpMax) hp = hpMax;
}

bool Entity::isAlive() const {
    return hp > 0;
}

void Entity::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}
