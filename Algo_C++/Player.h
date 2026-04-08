#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <vector>

class Player : public Entity {
private:
    int kills;
    int spares;
    int victories;

public:
    Player(const std::string& name);

    int getKills() const;
    int getSpares() const;
    int getVictories() const;

    void useItem(int index);
    void addKill();
    void addSpare();
    void addVictory();

    void display() const override;
    void displayStats() const;
    void displayInventory() const;
    void displayBestiary() const;
};

#endif
