#include "Monster.h"
#include <iostream>

Monster::Monster(const std::string& name, int hp, int atk, int def,
                  int mercyGoal, const std::vector<std::string>& actIDs)
    : Entity(name, hp, atk, def), mercy(0),
      mercyGoal(mercyGoal), actIDs(actIDs) {}

Monster::~Monster() {}

int Monster::getMercy() const { return mercy; }
int Monster::getMercyGoal() const { return mercyGoal; }
const std::vector<std::string>& Monster::getActIDs() const { return actIDs; }

void Monster::addMercy(int amount) {
    mercy += amount;
    if (mercy < 0) mercy = 0;
    if (mercy > mercyGoal) mercy = mercyGoal;
}

bool Monster::isMercyReached() const {
    return mercy >= mercyGoal;
}

void Monster::display() const {
    std::cout << name
              << " | HP: " << hp << "/" << hpMax
              << " | ATK: " << atk << " | DEF: " << def
              << " | Mercy: " << mercy << "/" << mercyGoal
              << std::endl;
}

int NormalMonster::getNumActs() const { return 2; }
int MiniBossMonster::getNumActs() const { return 3; }
int BossMonster::getNumActs() const { return 4; }
