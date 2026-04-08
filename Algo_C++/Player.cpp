#include "Player.h"
#include <iostream>

Player::Player(const std::string& name)
    : Entity(name, 100, 10, 5), kills(0), spares(0), victories(0) {}

int Player::getKills() const { return kills; }
int Player::getSpares() const { return spares; }
int Player::getVictories() const { return victories; }




void Player::addKill() { kills++; }
void Player::addSpare() { spares++; }
void Player::addVictory() { victories++; }


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

