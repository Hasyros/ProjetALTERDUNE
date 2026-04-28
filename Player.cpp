#include "Player.h"
#include <iostream>

using namespace std;

// Le joueur commence avec 100 HP, 10 ATK, 5 DEF par defaut
// (les stats du joueur ne sont pas dans un csv -> choix de design)
Player::Player(const string& nom)
    : Entity(nom, 100, 10, 5),
      kills(0), spares(0), victories(0) {}

vector<Item>&         Player::getInventory() { return inventory; }
vector<BestiaryEntry>& Player::getBestiary() { return bestiary; }
int Player::getKills()     const { return kills; }
int Player::getSpares()    const { return spares; }
int Player::getVictories() const { return victories; }

void Player::addItem(const Item& it) {
    inventory.push_back(it);
}

void Player::useItem(int index) {
    if (index < 0 || index >= (int)inventory.size()) {
        cout << "Item invalide..." << endl;
        return;
    }

    Item& it = inventory[index];

    if (it.getQuantity() <= 0) {
        cout << "Plus de " << it.getName() << " ! Faut faire les courses." << endl;
        return;
    }

    // Pour l'instant le seul type c'est HEAL
    if (it.getType() == "HEAL") {
        int soin = it.getValue();
        setHp(hp + soin);
        cout << "* Vous croquez/buvez " << it.getName()
             << " et recuperez " << soin << " HP."
             << "  (HP: " << hp << "/" << hpMax << ")" << endl;
    }
    // si on veut ajouter d'autres types plus tard, c'est ici

    it.use(); // decremente la quantite
}

void Player::addKill()    { kills++; }
void Player::addSpare()   { spares++; }
void Player::addVictory() { victories++; }

void Player::addBestiaryEntry(const BestiaryEntry& e) {
    bestiary.push_back(e);
}

void Player::display() const {
    cout << "[" << name << "]  HP: " << hp << "/" << hpMax << " DEF: " << def << endl;
}

void Player::displayStats() const {
    cout << "\n----- STATS DU PERSONNAGE -----\n";
    cout << "  Nom        : " << name << "\n";
    cout << "  HP         : " << hp << "/" << hpMax << "\n";
    cout << "  DEF        : " << def << "\n";
    cout << "  Tues       : " << kills    << "\n";
    cout << "  Epargnes   : " << spares   << "\n";
    cout << "  Victoires  : " << victories << "/10\n";
    cout << "-------------------------------\n" << endl;
}

void Player::displayInventory() const {
    cout << "\n----- INVENTAIRE -----\n";
    if (inventory.empty()) {
        cout << "  (vide... courage)\n";
    } else {
        for (int i = 0; i < (int)inventory.size(); i++) {
            cout << "  " << (i + 1) << ". ";
            inventory[i].display();
        }
    }
    cout << "----------------------\n" << endl;
}

void Player::displayBestiary() const {
    cout << "\n----- BESTIAIRE -----\n";
    if (bestiary.empty()) {
        cout << "  (vide pour l'instant, va te battre !)\n";
    } else {
        for (const auto& e : bestiary) {
            e.display();
        }
    }
    cout << "---------------------\n" << endl;
}
