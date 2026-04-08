#include "Player.h"
#include <iostream>

Player::Player(const std::string& name)
    : Entity(name, 100, 10, 5), kills(0), spares(0), victories(0) {}

std::vector<Item>& Player::getInventory() { return inventory; }
std::vector<BestiaryEntry>& Player::getBestiary() { return bestiary; }
int Player::getKills() const { return kills; }
int Player::getSpares() const { return spares; }
int Player::getVictories() const { return victories; }

void Player::addItem(const Item& item) {
    inventory.push_back(item);
}

void Player::useItem(int index) {
    if (index < 0 || index >= (int)inventory.size()) {
        std::cout << "Item invalide." << std::endl;
        return;
    }
    Item& item = inventory[index];
    if (item.getQuantity() <= 0) {
        std::cout << "Plus de " << item.getName() << " disponible !" << std::endl;
        return;
    }
    if (item.getType() == "HEAL") {
        int healed = item.getValue();
        setHp(hp + healed);
        std::cout << "Vous utilisez " << item.getName()
                  << " et recuperez " << healed << " HP !"
                  << " (HP: " << hp << "/" << hpMax << ")" << std::endl;
    }
    item.use();
}

void Player::addKill() { kills++; }
void Player::addSpare() { spares++; }
void Player::addVictory() { victories++; }

void Player::addBestiaryEntry(const BestiaryEntry& entry) {
    bestiary.push_back(entry);
}

void Player::display() const {
    std::cout << "=== " << name << " ===" << std::endl;
    std::cout << "HP: " << hp << "/" << hpMax << std::endl;
}

void Player::displayStats() const {
    std::cout << "\n===== STATISTIQUES =====" << std::endl;
    std::cout << "Nom       : " << name << std::endl;
    std::cout << "HP        : " << hp << "/" << hpMax << std::endl;
    std::cout << "Tues      : " << kills << std::endl;
    std::cout << "Epargnes  : " << spares << std::endl;
    std::cout << "Victoires : " << victories << "/10" << std::endl;
    std::cout << "========================\n" << std::endl;
}

void Player::displayInventory() const {
    std::cout << "\n===== INVENTAIRE =====" << std::endl;
    if (inventory.empty()) {
        std::cout << "  (vide)" << std::endl;
    }
    for (int i = 0; i < (int)inventory.size(); i++) {
        std::cout << "  " << (i + 1) << ". ";
        inventory[i].display();
    }
    std::cout << "======================\n" << std::endl;
}

void Player::displayBestiary() const {
    std::cout << "\n===== BESTIAIRE =====" << std::endl;
    if (bestiary.empty()) {
        std::cout << "  (aucun monstre vaincu)" << std::endl;
    }
    for (const auto& entry : bestiary) {
        entry.display();
    }
    std::cout << "=====================\n" << std::endl;
}
