#ifndef ITEM_H
#define ITEM_H

#include <string>

// Un item (potion, snack, etc.) que le joueur peut utiliser
class Item {
private:
    std::string name;
    std::string type;   // pour l'instant que "HEAL"
    int value;          // ex: nb de HP rendus
    int quantity;

public:
    Item(const std::string& nom, const std::string& t, int val, int qte);

    std::string getName() const;
    std::string getType() const;
    int getValue() const;
    int getQuantity() const;

    void use();           // decremente la quantite
    void display() const; // affichage propre
};

#endif
