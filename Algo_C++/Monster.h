#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include "BestiaryEntry.h"
#include <vector>
#include <string>

class Monster : public Entity {
protected:
    Category category;
    int mercy;
    int mercyGoal;
    std::vector<std::string> actIDs;

public:
    Monster(const std::string& name, int hp, int atk, int def,
            Category category, int mercyGoal, const std::vector<std::string>& actIDs);
    virtual ~Monster();

    Category getCategory() const;
    int getMercy() const;
    int getMercyGoal() const;
    const std::vector<std::string>& getActIDs() const;

    void addMercy(int amount);
    bool isMercyReached() const;

    void display() const override;
    virtual int getNumActs() const = 0;
};

class NormalMonster : public Monster {
public:
    using Monster::Monster;
    int getNumActs() const override;
};

class MiniBossMonster : public Monster {
public:
    using Monster::Monster;
    int getNumActs() const override;
};

class BossMonster : public Monster {
public:
    using Monster::Monster;
    int getNumActs() const override;
};

#endif
