#ifndef ACTACTION_H
#define ACTACTION_H

#include <string>

// Une action ACT du jeu (style Undertale)
// id = COMPLIMENT, JOKE, INSULT...
// text = la phrase rigolote a afficher
// mercyEffect = + si ca apaise, - si ca enerve
class ActAction {
private:
    std::string id;
    std::string text;
    int mercyEffect;

public:
    ActAction();   // ctor par defaut, requis pour std::map
    ActAction(const std::string& id, const std::string& text, int effet);

    std::string getId() const;
    std::string getText() const;
    int getMercyEffect() const;
};

#endif
