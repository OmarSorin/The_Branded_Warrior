#ifndef CHARACTER_H
#define CHARACTER_H

#include "Inventory.h"
#include "Potion.h"
#include "Weapon.h"
#include <ostream>
#include <string>

class Character {
private:
  std::string name;
  int hp;
  int maxHp;
  int level;
  int xp;
  Inventory inventory;
  int posX;
  int posY;

  int xpForNextLevel() const;

  // lvl up, daca primeste mult xp odata mai multe lvl de aia return levelsGained
  int checkLevelUp();

public:
  // Arma primita este adaugata ca slot 0 in inventar (= arma echipata)
  explicit Character(const std::string &name = "Hero", int hp = 100, int level = 1,
            const Weapon &weapon = Weapon(), const Inventory &inv = Inventory(),
            int posX = 0, int posY = 0);

  const std::string &getName() const { return name; }
  int getHp() const { return hp; }
  int getMaxHp() const { return maxHp; }
  int getLevel() const { return level; }
  int getXp() const { return xp; }
  int getXpForNextLevel() const;
  bool isAlive() const { return hp > 0; }

  const Weapon &getEquippedWeapon() const { return inventory.getWeapon(0); }

  const Inventory &getInventory() const { return inventory; }

  int getX() const { return posX; }
  int getY() const { return posY; }
  void setPosition(int x, int y);
  void move(int dx, int dy);

  // Adauga arma in inventar
  void pickUpWeapon(const Weapon &w);

  // Ataca, returneaza dmg dat
  int attackTarget(Character &target);

  // Primeste damage, returneaza true daca personajul a murit
  bool takeDamage(int amount);

  // Foloseste o potiune, returneaza cate HP s-au vindecat (0 daca nu s-a vindecat)
  int heal(const Potion &potion);

  // Castiga XP si verifica level up, returneaza nr. de niveluri castigate
  int gainXp(int amount);
};

std::ostream &operator<<(std::ostream &os, const Character &c);

#endif // CHARACTER_H
