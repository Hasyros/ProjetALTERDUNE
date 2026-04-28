#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <limits>

using namespace std;

// =====================================================================
//   Constructeur / Destructeur
// =====================================================================
Game::Game() : player(nullptr) {}

Game::~Game() {
    delete player;
    for (auto* m : monsterPool) delete m;
}

// =====================================================================
//   Catalogue d'actions ACT (10 actions, 2 negatives -> spec OK)
// =====================================================================
void Game::initActCatalog() {
    // positives (apaisent le monstre)
    actCatalog["COMPLIMENT"] = ActAction("COMPLIMENT",
        "* Vous complimentez le monstre sur sa coupe de cheveux. Il rougit.", 25);
    actCatalog["JOKE"] = ActAction("JOKE",
        "* Vous racontez une blague nulle. Le monstre rit malgre lui !", 20);
    actCatalog["DISCUSS"] = ActAction("DISCUSS",
        "* Vous parlez de la pluie et du beau temps. Le monstre acquiesce.", 15);
    actCatalog["PET"] = ActAction("PET",
        "* Vous caressez doucement le monstre. Il se met a ronronner ?!", 30);
    actCatalog["DANCE"] = ActAction("DANCE",
        "* Vous lancez un Macarena improvise. Le monstre est sous le choc.", 20);
    actCatalog["OBSERVE"] = ActAction("OBSERVE",
        "* Vous observez le monstre. Il se sent vu, compris.", 10);
    actCatalog["OFFER_SNACK"] = ActAction("OFFER_SNACK",
        "* Vous offrez un croissant au monstre. Il a la larme a l'oeil.", 25);
    actCatalog["REASON"] = ActAction("REASON",
        "* Vous expliquez calmement qu'on peut juste rentrer chez soi.", 15);

    // negatives (l'enervent) -> minimum 2 d'apres l'enonce
    actCatalog["INSULT"] = ActAction("INSULT",
        "* Vous insultez le monstre. Mauvaise idee. Il fume des oreilles.", -20);
    actCatalog["THREATEN"] = ActAction("THREATEN",
        "* Vous menacez le monstre. Genial, il est encore plus enerve.", -15);
}

// petit helper pour nettoyer les espaces autour des champs csv
static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// helper pour vider le buffer apres un cin >> int (sinon le \n traine)
static void clearStdin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// =====================================================================
//   Lecture de items.csv
//   Format : nom;type;valeur;quantite
// =====================================================================
void Game::loadItems(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERREUR : impossible d'ouvrir " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string nom, type, valStr, qtyStr;

        if (!getline(ss, nom,    ';')) continue;
        if (!getline(ss, type,   ';')) continue;
        if (!getline(ss, valStr, ';')) continue;
        if (!getline(ss, qtyStr, ';')) continue;

        try {
            int valeur   = stoi(trim(valStr));
            int quantite = stoi(trim(qtyStr));
            player->addItem(Item(trim(nom), trim(type), valeur, quantite));
        } catch (...) {
            // ligne mal formee -> on ignore et on continue
            cerr << "(warn) ligne ignoree dans " << filename << " : " << line << endl;
        }
    }
    file.close();
}

// =====================================================================
//   Lecture de monsters.csv
//   Format : categorie;nom;hp;atk;def;mercyGoal;act1;act2;act3;act4
// =====================================================================
void Game::loadMonsters(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERREUR : impossible d'ouvrir " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string catStr, nom, hpStr, atkStr, defStr, mercyStr;
        string a1, a2, a3, a4;

        if (!getline(ss, catStr,   ';')) continue;
        if (!getline(ss, nom,      ';')) continue;
        if (!getline(ss, hpStr,    ';')) continue;
        if (!getline(ss, atkStr,   ';')) continue;
        if (!getline(ss, defStr,   ';')) continue;
        if (!getline(ss, mercyStr, ';')) continue;
        // les 4 actes sont optionnels (NORMAL n'en a que 2 par ex)
        getline(ss, a1, ';');
        getline(ss, a2, ';');
        getline(ss, a3, ';');
        getline(ss, a4, ';');

        try {
            Category cat = stringToCategory(trim(catStr));
            int pv    = stoi(trim(hpStr));
            int atk   = stoi(trim(atkStr));
            int def   = stoi(trim(defStr));
            int goal  = stoi(trim(mercyStr));

            // on collecte les actes valides (on ignore "-" et les vides)
            vector<string> acts;
            for (const string& a : {a1, a2, a3, a4}) {
                string t = trim(a);
                if (!t.empty() && t != "-") acts.push_back(t);
            }

            // on instancie la bonne sous classe selon la categorie (factory simple)
            Monster* m = nullptr;
            if (cat == Category::NORMAL)        m = new NormalMonster   (trim(nom), pv, atk, def, cat, goal, acts);
            else if (cat == Category::MINIBOSS) m = new MiniBossMonster (trim(nom), pv, atk, def, cat, goal, acts);
            else                                m = new BossMonster     (trim(nom), pv, atk, def, cat, goal, acts);

            monsterPool.push_back(m);
        } catch (...) {
            cerr << "(warn) ligne ignoree dans " << filename << " : " << line << endl;
        }
    }
    file.close();
}

// =====================================================================
//   Helper : clone un monstre du pool pour le combat
//   (sinon on modifierait le HP / Mercy de l'original)
// =====================================================================
Monster* Game::cloneMonster(Monster* base) const {
    Category c = base->getCategory();
    if (c == Category::NORMAL)
        return new NormalMonster(base->getName(), base->getHpMax(), base->getAtk(),
                                 base->getDef(), c, base->getMercyGoal(), base->getActIDs());
    if (c == Category::MINIBOSS)
        return new MiniBossMonster(base->getName(), base->getHpMax(), base->getAtk(),
                                   base->getDef(), c, base->getMercyGoal(), base->getActIDs());
    return new BossMonster(base->getName(), base->getHpMax(), base->getAtk(),
                           base->getDef(), c, base->getMercyGoal(), base->getActIDs());
}

// =====================================================================
//   Sous menus
// =====================================================================
void Game::showBestiary() { player->displayBestiary(); }
void Game::showStats()    { player->displayStats(); }

void Game::showItems() {
    player->displayInventory();
    cout << "Utiliser un item ? (numero, 0 pour revenir) : ";
    int choix;
    if (!(cin >> choix)) { clearStdin(); return; }
    clearStdin();
    if (choix > 0) player->useItem(choix - 1);
}

// =====================================================================
//   Le coeur du jeu : un combat
// =====================================================================
void Game::startCombat() {
    if (monsterPool.empty()) {
        cout << "Aucun monstre dispo... bizarre." << endl;
        return;
    }

    // RNG static pour qu'on ait le meme generateur partout
    static mt19937 rng(random_device{}());

    // tirage aleatoire d'un monstre dans le pool
    uniform_int_distribution<int> pick(0, (int)monsterPool.size() - 1);
    Monster* enemy = cloneMonster(monsterPool[pick(rng)]);

    cout << "\n================================\n";
    cout << " Un " << enemy->getName() << " sauvage apparait !\n";
    cout << "================================\n" << endl;

    // boucle de combat tant que les deux sont vivants
    while (player->isAlive() && enemy->isAlive()) {
        cout << "--- Etat ---\n";
        player->display();
        enemy->display();
        cout << "\n   FIGHT   ACT   ITEM   MERCY\n";
        cout << "Action : ";

        string choix;
        cin >> choix;
        // on met en majuscules pour accepter "fight", "Fight", etc.
        transform(choix.begin(), choix.end(), choix.begin(), ::toupper);

        // ===== FIGHT =====
        if (choix == "FIGHT") {
            uniform_int_distribution<int> dmgD(0, enemy->getHpMax());
            int dmg = dmgD(rng);

            if (dmg - enemy->getDef() <= 0) {
                cout << "* Coup rate ! Honteux." << endl;
            } else {
                enemy->takeDamage(dmg - enemy->getDef());
                cout << "* Vous infligez " << dmg << " degats a "
                     << enemy->getName() << "."
                     << "  (HP: " << enemy->getHp() << "/" << enemy->getHpMax() << ")"
                     << endl;
            }

            if (!enemy->isAlive()) {
                cout << "\n>>> " << enemy->getName() << " est K.O. ! <<<\n" << endl;
                player->addKill();
                player->addVictory();
                player->addBestiaryEntry(BestiaryEntry(
                    enemy->getName(), enemy->getCategory(),
                    enemy->getHpMax(), enemy->getAtk(), enemy->getDef(),
                    false));  // tue
                break;
            }
        }
        // ===== ACT =====
        else if (choix == "ACT") {
            const auto& acts = enemy->getActIDs();
            int n = enemy->getNumActs();   // 2 / 3 / 4 selon la categorie
            // on borne n par la taille reelle (cas ou le csv en a moins)
            if (n > (int)acts.size()) n = (int)acts.size();

            cout << "Actions dispo :\n";
            for (int i = 0; i < n; i++) {
                cout << "  " << (i + 1) << ". " << acts[i] << "\n";
            }
            cout << "Choix : ";
            int a;
            if (!(cin >> a)) { clearStdin(); continue; }
            a--;

            if (a >= 0 && a < n) {
                auto it = actCatalog.find(acts[a]);
                if (it != actCatalog.end()) {
                    cout << it->second.getText() << endl;
                    enemy->addMercy(it->second.getMercyEffect());
                    cout << "  (Mercy: " << enemy->getMercy()
                         << "/" << enemy->getMercyGoal() << ")" << endl;
                } else {
                    cout << "Action " << acts[a] << " introuvable dans le catalogue !" << endl;
                }
            } else {
                cout << "Numero invalide." << endl;
                continue;  // pas de tour consomme si choix invalide
            }
        }
        // ===== ITEM =====
        else if (choix == "ITEM") {
            player->displayInventory();
            cout << "Quel item ? (numero, 0 pour annuler) : ";
            int n;
            if (!(cin >> n)) { clearStdin(); continue; }
            if (n <= 0) continue;     // annule -> pas de tour consomme
            player->useItem(n - 1);
        }
        // ===== MERCY =====
        else if (choix == "MERCY") {
            if (enemy->isMercyReached()) {
                cout << "\n* Vous epargnez " << enemy->getName()
                     << ". La paix... enfin. *\n" << endl;
                player->addSpare();
                player->addVictory();
                player->addBestiaryEntry(BestiaryEntry(
                    enemy->getName(), enemy->getCategory(),
                    enemy->getHpMax(), enemy->getAtk(), enemy->getDef(),
                    true));  // epargne
                break;
            } else {
                cout << "Mercy pas assez haute (" << enemy->getMercy()
                     << "/" << enemy->getMercyGoal()
                     << "). Le monstre te regarde de travers." << endl;
            }
        }
        else {
            cout << "Hein ? Tape FIGHT / ACT / ITEM / MERCY." << endl;
            continue;  // pas de tour consomme si commande invalide
        }

        // ===== Tour du monstre (s'il est encore vivant) =====
        if (enemy->isAlive()) {
            uniform_int_distribution<int> mDmgD(0, player->getHpMax());
            int mdmg = mDmgD(rng);

            if (mdmg == 0) {
                cout << "* " << enemy->getName() << " rate son attaque, ouf." << endl;
            } else {
                player->takeDamage(mdmg/player->getDef());
                cout << "* " << enemy->getName() << " inflige " << mdmg << " degats !"
                     << "  (HP: " << player->getHp() << "/" << player->getHpMax() << ")"
                     << endl;
            }

            if (!player->isAlive()) {
                cout << "\n>>> Vous etes mort... GAME OVER <<<\n" << endl;
            }
        }
    }

    delete enemy;
}

// =====================================================================
//   Fin de partie : 3 fins selon kills/spares
// =====================================================================
void Game::checkEnding() {
    cout << "\n========================================\n";
    cout <<   "    10 VICTOIRES ! Bien joue.\n";
    cout <<   "========================================\n" << endl;

    if (player->getSpares() == 0 && player->getKills() > 0) {
        cout << ">>> FIN GENOCIDAIRE <<<\n";
        cout << "Tu n'as epargne personne. Brutal.\n";
    }
    else if (player->getKills() == 0 && player->getSpares() > 0) {
        cout << ">>> FIN PACIFISTE <<<\n";
        cout << "Tu as epargne tout le monde. La paix regne.\n";
    }
    else {
        cout << ">>> FIN NEUTRE <<<\n";
        cout << "Mi-figue mi-raisin, comme la vraie vie.\n";
    }

    cout << "\nBilan : " << player->getKills() << " tues, "
         << player->getSpares() << " epargnes.\n" << endl;
}

// =====================================================================
//   Menu principal
// =====================================================================
void Game::mainMenu() {
    while (true) {
        // conditions de sortie
        if (player->getVictories() >= 10) { checkEnding(); break; }
        if (!player->isAlive()) {
            cout << "Ton perso est mort, fin de la partie." << endl;
            break;
        }

        cout << "\n========== MENU ==========\n";
        cout << "  1. Bestiaire\n";
        cout << "  2. Demarrer un combat\n";
        cout << "  3. Statistiques du personnage\n";
        cout << "  4. Items\n";
        cout << "  5. Quitter\n";
        cout << "==========================\n";
        cout << "Choix : ";

        int choix;
        if (!(cin >> choix)) {
            clearStdin();
            cout << "Faut entrer un nombre quand meme..." << endl;
            continue;
        }
        clearStdin();

        switch (choix) {
            case 1: showBestiary(); break;
            case 2: startCombat();  break;
            case 3: showStats();    break;
            case 4: showItems();    break;
            case 5:
                cout << "Bye !" << endl;
                return;
            default:
                cout << "Pas un choix valide." << endl;
        }
    }
}

// =====================================================================
//   Lancement du jeu
// =====================================================================
void Game::run() {
    cout << "========================================\n";
    cout << "        BIENVENUE DANS ALTERDUNE\n";
    cout << "========================================\n" << endl;

    cout << "Entre le nom de ton heros : ";
    string nom;
    getline(cin, nom);
    if (nom.empty()) nom = "Frisk"; // au cas ou

    player = new Player(nom);

    // setup du jeu
    initActCatalog();
    loadItems("items.csv");
    loadMonsters("monsters.csv");

    cout << "\n--- Resume de depart ---" << endl;
    player->display();
    player->displayInventory();

    // c'est parti
    mainMenu();
}
