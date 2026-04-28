#include "Player.h"
#include <iostream>

using namespace std;

// Le joueur commence avec 100 HP, 10 ATK, 5 DEF par defaut
Player::Player(const string& nom)
    : Entity(nom, 100, 10, 5),
      kills(0), spares(0), victories(0),
      defBonusActive(0), defBonusTurns(0) {}

vector<Item>&          Player::getInventory() { return inventory; }
vector<BestiaryEntry>& Player::getBestiary()  { return bestiary; }
int  Player::getKills()     const { return kills; }
int  Player::getSpares()    const { return spares; }
int  Player::getVictories() const { return victories; }
bool Player::hasDefBonus()  const { return defBonusTurns > 0; }

// -----------------------------------------------------------------------
//  applyDefBonus : ajoute directement +bonus a def et arme le compteur.
//  Si un buff est deja actif, on retire d'abord l'ancien pour eviter
//  les empilements infinis.
// -----------------------------------------------------------------------
void Player::applyDefBonus(int bonus, int turns) {
    // retirer l'eventuel buff precedent
    if (defBonusActive > 0) {
        def -= defBonusActive;
    }
    // appliquer le nouveau
    defBonusActive = bonus;
    defBonusTurns  = turns;
    def += defBonusActive;
}

// -----------------------------------------------------------------------
//  tickDefBonus : a appeler une fois par tour apres l'action du joueur.
//  Decremente le compteur et retire le bonus quand il expire.
// -----------------------------------------------------------------------
void Player::tickDefBonus() {
    if (defBonusTurns <= 0) return;

    defBonusTurns--;
    if (defBonusTurns == 0) {
        def -= defBonusActive;
        defBonusActive = 0;
        cout << "* L'effet du Talisman se dissipe. DEF revient a la normale ("
             << def << ")." << endl;
    } else {
        cout << "  (Buff DEF actif encore " << defBonusTurns << " tour(s))" << endl;
    }
}

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

    if (it.getType() == "HEAL") {
        int soin = it.getValue();
        setHp(hp + soin);
        cout << "* Vous croquez/buvez " << it.getName()
             << " et recuperez " << soin << " HP."
             << "  (HP: " << hp << "/" << hpMax << ")" << endl;
    }
    else if (it.getType() == "BOOST_DEF") {
        int bonus = it.getValue();
        applyDefBonus(bonus, 3);
        cout << "* Vous equippez le " << it.getName()
             << ". DEF augmente de +" << bonus
             << " pendant 3 tours !"
             << "  (DEF: " << def << ")" << endl;
    }
    else {
        cout << "Vous utilisez " << it.getName() << ". Rien ne se passe... bizarre." << endl;
    }

    it.use();
}

void Player::addKill()    { kills++; }
void Player::addSpare()   { spares++; }
void Player::addVictory() { victories++; }

void Player::addBestiaryEntry(const BestiaryEntry& e) {
    bestiary.push_back(e);
}

void Player::display() const {
    cout << "[" << name << "]  HP: " << hp << "/" << hpMax
         << "  DEF: " << def;
    if (defBonusTurns > 0)
        cout << " (+" << defBonusActive << " tmp, " << defBonusTurns << " tour(s))";
    cout << endl;
}

void Player::displayStats() const {
    cout << "\n----- STATS DU PERSONNAGE -----\n";
    cout << "  Nom        : " << name << "\n";
    cout << "  HP         : " << hp << "/" << hpMax << "\n";
    cout << "  ATK        : " << atk << "\n";
    cout << "  DEF        : " << def;
    if (defBonusTurns > 0)
        cout << " (+" << defBonusActive << " temporaire, " << defBonusTurns << " tour(s) restant(s))";
    cout << "\n";
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