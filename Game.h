#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Monster.h"
#include "ActAction.h"
#include <vector>
#include <map>
#include <string>

// Classe principale qui gere tout le jeu :
//  - le joueur
//  - le pool de monstres charge depuis monsters.csv
//  - le catalogue d'actions ACT (cree dans le code)
//  - les menus, les combats, les fins
class Game {
private:
    Player* player;
    std::vector<Monster*> monsterPool;       // tous les monstres possibles
    std::map<std::string, ActAction> actCatalog;  // les actions ACT (au moins 8)

    // setup
    void initActCatalog();
    void loadItems(const std::string& filename);
    void loadMonsters(const std::string& filename);

    // menus
    void mainMenu();
    void startCombat();
    void showBestiary();
    void showStats();
    void showItems();

    // fin
    void checkEnding();

    // helper : clone un monstre du pool pour pas modifier l'original
    Monster* cloneMonster(Monster* base) const;

public:
    Game();
    ~Game();
    void run();
};

#endif
