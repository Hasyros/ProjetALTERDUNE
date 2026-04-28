#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include "BestiaryEntry.h"
#include <vector>
#include <string>

// Classe abstraite Monster qui herite d'Entity
// On a 3 sous classes : NormalMonster, MiniBossMonster, BossMonster
// chacune redefinit getNumActs() -> 2 / 3 / 4 (polymorphisme !)
class Monster : public Entity {
protected:
    Category category;
    int mercy;
    int mercyGoal;
    std::vector<std::string> actIDs;  // les ids des actions dispo (de monsters.csv)

public:
    Monster(const std::string& nom, int pv, int atk, int def,
            Category cat, int goal, const std::vector<std::string>& acts);
    virtual ~Monster();

    Category getCategory() const;
    int getMercy() const;
    int getMercyGoal() const;
    const std::vector<std::string>& getActIDs() const;

    void addMercy(int amount);     // ajoute (ou retire) avec bornage 0..mercyGoal
    bool isMercyReached() const;   // pour savoir si on peut MERCY

    void display() const override;       // implemente la virtuelle pure d'Entity
    virtual int getNumActs() const = 0;  // pure virtuelle = polymorphisme
};

// --- Sous classes ---
// On utilise "using Monster::Monster" pour heriter du constructeur (C++11)

class NormalMonster : public Monster {
public:
    using Monster::Monster;
    int getNumActs() const override; // 2
};

class MiniBossMonster : public Monster {
public:
    using Monster::Monster;
    int getNumActs() const override; // 3
};

class BossMonster : public Monster {
public:
    using Monster::Monster;
    int getNumActs() const override; // 4
};

#endif
