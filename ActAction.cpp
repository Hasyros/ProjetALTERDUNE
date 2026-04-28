#include "ActAction.h"

using namespace std;

ActAction::ActAction() : id(""), text(""), mercyEffect(0) {}

ActAction::ActAction(const string& i, const string& t, int effet)
    : id(i), text(t), mercyEffect(effet) {}

string ActAction::getId() const         { return id; }
string ActAction::getText() const       { return text; }
int    ActAction::getMercyEffect() const { return mercyEffect; }
