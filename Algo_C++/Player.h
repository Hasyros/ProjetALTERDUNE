#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Item.h"
#include "BestiaryEntry.h"
#include <vector>

class Player : public Entity {
private:
    std::vector<Item> inventory;
    int kills;
    int spares;
    int victories;
    std::vector<BestiaryEntry> bestiary;

public:
    Player(const std::string& name);

    std::vector<Item>& getInventory();
    std::vector<BestiaryEntry>& getBestiary();
    int getKills() const;
    int getSpares() const;
    int getVictories() const;

    void addItem(const Item& item);
    void useItem(int index);
    void addKill();
    void addSpare();
    void addVictory();
    void addBestiaryEntry(const BestiaryEntry& entry);

    void display() const override;
    void displayStats() const;
    void displayInventory() const;
    void displayBestiary() const;
};

#endif
