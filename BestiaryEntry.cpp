#include "BestiaryEntry.h"
#include <iostream>

using namespace std;

string categoryToString(Category cat) {
    switch (cat) {
        case Category::NORMAL:   return "NORMAL";
        case Category::MINIBOSS: return "MINIBOSS";
        case Category::BOSS:     return "BOSS";
    }
    return "???";
}

Category stringToCategory(const string& s) {
    if (s == "MINIBOSS") return Category::MINIBOSS;
    if (s == "BOSS")     return Category::BOSS;
    return Category::NORMAL; // fallback par defaut
}

BestiaryEntry::BestiaryEntry(const string& nom, Category cat,
                             int pvMax, int attaque, int defense, bool epargne)
    : monsterName(nom), category(cat),
      hpMax(pvMax), atk(attaque), def(defense), wasSpared(epargne) {}

void BestiaryEntry::display() const {
    cout << "  - " << monsterName
         << " [" << categoryToString(category) << "]"
         << "  HP:" << hpMax
         << " ATK:" << atk
         << " DEF:" << def
         << "  =>  " << (wasSpared ? "Epargne" : "Tue")
         << endl;
}
