#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Item.h"
#include "BestiaryEntry.h"
#include <vector>

// Le joueur : herite d'Entity
// Contient : un inventaire, un bestiaire, et des compteurs (kills/spares/victories)
class Player : public Entity {
private:
    std::vector<Item> inventory;
    int kills;       // monstres tues
    int spares;      // monstres epargnes
    int victories;   // total combats gagnes (kills + spares)
    std::vector<BestiaryEntry> bestiary;

public:
    Player(const std::string& nom);

    // accesseurs
    std::vector<Item>& getInventory();
    std::vector<BestiaryEntry>& getBestiary();
    int getKills() const;
    int getSpares() const;
    int getVictories() const;

    // modifs
    void addItem(const Item& it);
    void useItem(int index);
    void addKill();
    void addSpare();
    void addVictory();
    void addBestiaryEntry(const BestiaryEntry& e);

    // affichages (override de la virtuelle pure d'Entity)
    void display() const override;
    void displayStats() const;
    void displayInventory() const;
    void displayBestiary() const;
};

#endif
