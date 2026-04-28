# ALTERDUNE

Mini-RPG type Undertale en C++, fait pour le projet de POO de l'ESILV (S2 2025-2026).

Inspire (clairement) d'Undertale : on peut soit taper les monstres, soit les
charmer avec des actions ACT pour faire monter leur jauge **Mercy** et les
epargner. Selon la maniere dont on finit la partie, on debloque une fin
differente (genocidaire / pacifiste / neutre).

---

## Compilation et lancement

Depuis le dossier du projet, dans un terminal Linux/WSL :

```bash
make
./alterdune
```

ou directement :

```bash
make run
```

Pour nettoyer les fichiers compiles :

```bash
make clean
```

Si pas de `make`, en ligne :

```bash
g++ -std=c++17 -Wall -Wextra -o alterdune main.cpp Game.cpp Player.cpp \
    Monster.cpp Entity.cpp Item.cpp ActAction.cpp BestiaryEntry.cpp
```

---

## Structure des fichiers

```
.
├── main.cpp              # point d'entree
├── Game.h / .cpp         # orchestre tout (menus, combat, lecture fichiers)
├── Entity.h / .cpp       # classe abstraite (HP, ATK, DEF, ...)
├── Player.h / .cpp       # joueur (herite d'Entity)
├── Monster.h / .cpp      # monstre abstrait + 3 sous classes
├── Item.h / .cpp         # objets de l'inventaire
├── ActAction.h / .cpp    # actions ACT du combat
├── BestiaryEntry.h / .cpp # entrees du bestiaire
├── items.csv             # inventaire de depart
├── monsters.csv          # base de monstres pour les combats
├── Makefile
├── README.md
└── UML_ALTERDUNE.drawio  # diagramme UML (a ouvrir avec draw.io)
```

---

## Architecture (resume rapide)

```
        Entity (abstraite)
        /              \
      Player          Monster (abstraite)
                    /     |       \
              Normal   MiniBoss    Boss
              (2 ACT)  (3 ACT)    (4 ACT)
```

- **Entity** : la classe mere. Centralise nom, HP, ATK, DEF.
- **Player** : herite d'Entity, ajoute inventaire + bestiaire + compteurs.
- **Monster** : abstraite, ajoute Mercy + categorie + IDs d'actions.
- **NormalMonster / MiniBossMonster / BossMonster** : redefinissent
  `getNumActs()` -> 2 / 3 / 4. C'est le polymorphisme demande.

### Concepts POO couverts

| Concept         | Ou ?                                              |
|-----------------|---------------------------------------------------|
| Encapsulation   | attributs `private` + getters/setters partout     |
| Heritage        | Entity -> Player, Entity -> Monster -> 3 enfants  |
| Classe abstraite| `Entity::display()=0`, `Monster::getNumActs()=0`  |
| Polymorphisme   | `getNumActs()` retourne 2/3/4 selon la sous classe|
| Composition     | `Player` contient `vector<Item>`, `vector<BestiaryEntry>`, `Game` contient `map<string, ActAction>` |
| Lecture fichier | `Game::loadItems`, `Game::loadMonsters`           |

---

## Format des fichiers CSV

### `items.csv`
```
nom;type;valeur;quantite
```
Pour l'instant le seul `type` gere est `HEAL`.

### `monsters.csv`
```
categorie;nom;hp;atk;def;mercyGoal;act1;act2;act3;act4
```
- NORMAL : seuls act1 et act2 sont utilises (les autres = `-`)
- MINIBOSS : act1 a act3
- BOSS : les 4 actes
- les IDs d'actes doivent exister dans le catalogue (voir
  `Game::initActCatalog`)

---

## Actions ACT pre-definies

Le catalogue contient **10 actions** (8+ minimum demande), dont **2 negatives**
(elles font BAISSER Mercy au lieu de l'augmenter, c'est dans la spec) :

| ID            | Effet Mercy | Type     |
|---------------|-------------|----------|
| COMPLIMENT    | +25         | positif  |
| JOKE          | +20         | positif  |
| DISCUSS       | +15         | positif  |
| PET           | +30         | positif  |
| DANCE         | +20         | positif  |
| OBSERVE       | +10         | positif  |
| OFFER_SNACK   | +25         | positif  |
| REASON        | +15         | positif  |
| INSULT        | -20         | NEGATIF  |
| THREATEN      | -15         | NEGATIF  |

---

## Combat (rappel des regles)

A chaque tour le joueur choisit :

- **FIGHT** : attaque directe. Degats = `rand(0, HP_max_ennemi)`.
  Si 0 -> coup rate.
- **ACT** : action drole, modifie Mercy (positif ou negatif).
- **ITEM** : utilise un item de l'inventaire (HEAL).
- **MERCY** : si Mercy >= mercyGoal, on epargne le monstre. Sinon il
  refuse.

Si le monstre est encore vivant a la fin du tour, il riposte avec la
meme formule (`rand(0, HP_max_joueur)`). 0 HP -> Game Over.

La partie se termine a 10 victoires, et la fin depend des choix :

- 0 epargne, X tues -> **Genocidaire**
- X epargnes, 0 tue -> **Pacifiste**
- les deux -> **Neutre**

---

## Notes / choix techniques

- L'aleatoire utilise `<random>` (mt19937) plutot que `rand()` -> propre.
- Le pool de monstres charge depuis le csv n'est **jamais modifie pendant
  un combat** : on **clone** le monstre tire pour preserver les originaux
  (voir `Game::cloneMonster`).
- Le buffer de `cin` est nettoye apres chaque saisie d'entier pour
  eviter les bugs avec le `\n` qui traine.
- `Mercy` est borne entre 0 et `mercyGoal` (`Monster::addMercy`).
- Gestion d'erreurs minimale sur les CSV : fichier manquant -> arret
  propre, ligne mal formee -> ignore avec un warning.

---

## TODO / ameliorations possibles (bonus)

- [ ] Ajouter un type d'item `BOOST` (augmente ATK temporairement)
- [ ] Sauvegarde / chargement de partie
- [ ] Sons/couleurs ANSI pour rendre l'interface plus vivante
- [ ] Plus d'actions ACT
- [ ] Difficulte progressive selon le nombre de victoires
