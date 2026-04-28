#include "Item.h"
#include <iostream>

using namespace std;

Item::Item(const string& nom, const string& t, int val, int qte)
    : name(nom), type(t), value(val), quantity(qte) {}

string Item::getName() const     { return name; }
string Item::getType() const     { return type; }
int    Item::getValue() const    { return value; }
int    Item::getQuantity() const { return quantity; }

void Item::use() {
    if (quantity > 0) quantity--;
}

void Item::display() const {
    cout << name << " (" << type << ")"
         << " - rend " << value << " HP"
         << " - x" << quantity << endl;
}
