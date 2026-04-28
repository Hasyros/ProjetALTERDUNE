#ifndef BESTIARYENTRY_H
#define BESTIARYENTRY_H

#include <string>

// Les 3 categories de monstres (impacte le nb d'actions ACT dispo)
enum class Category { NORMAL, MINIBOSS, BOSS };

// utilitaires de conversion
std::string categoryToString(Category cat);
Category stringToCategory(const std::string& s);

// Une entree dans le bestiaire = un monstre vaincu
// On stocke ses stats au moment du combat + s'il a ete tue ou epargne
class BestiaryEntry {
private:
    std::string monsterName;
    Category category;
    int hpMax;
    int atk;
    int def;
    bool wasSpared;   // true = epargne, false = tue

public:
    BestiaryEntry(const std::string& nom, Category cat,
                  int pvMax, int attaque, int defense, bool epargne);

    void display() const;
};

#endif
