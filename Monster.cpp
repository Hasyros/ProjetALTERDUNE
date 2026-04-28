#include "Monster.h"
#include <iostream>

using namespace std;

Monster::Monster(const string& nom, int pv, int attaque, int defense,
                 Category cat, int goal, const vector<string>& acts)
    : Entity(nom, pv, attaque, defense),
      category(cat), mercy(0), mercyGoal(goal), actIDs(acts) {}

Monster::~Monster() {}

Category Monster::getCategory() const            { return category; }
int      Monster::getMercy() const               { return mercy; }
int      Monster::getMercyGoal() const           { return mercyGoal; }
const vector<string>& Monster::getActIDs() const { return actIDs; }

void Monster::addMercy(int amount) {
    mercy += amount;
    // bornage : Mercy ne peut pas etre < 0 ni > mercyGoal
    if (mercy < 0)         mercy = 0;
    if (mercy > mercyGoal) mercy = mercyGoal;
}

bool Monster::isMercyReached() const {
    return mercy >= mercyGoal;
}

void Monster::display() const {
    cout << name << " [" << categoryToString(category) << "]"
         << "  HP: " << hp << "/" << hpMax
         << "  ATK:" << atk << " DEF:" << def
         << "  Mercy: " << mercy << "/" << mercyGoal
         << endl;
}

// le polymorphisme tant attendu :
int NormalMonster::getNumActs()    const { return 2; }
int MiniBossMonster::getNumActs()  const { return 3; }
int BossMonster::getNumActs()      const { return 4; }
