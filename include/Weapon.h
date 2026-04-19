#ifndef WEAPON_H
#define WEAPON_H

#include <ostream>
#include <string>

class Weapon {
private:
  std::string name;
  int damage;
  int durability;
  int maxDurability;

  // roll crit
  int rollCritical() const;

public:
  Weapon(const std::string &name = "Fists", int damage = 2,
         int durability = 100);

  const std::string &getName() const { return name; }
  int getDamage() const { return damage; }
  int getDurability() const { return durability; }
  int getMaxDurability() const { return maxDurability; }

  bool isBroken() const { return durability <= 0; }

  int attack();

  void repair(int amount);
};

std::ostream &operator<<(std::ostream &os, const Weapon &w);

#endif // WEAPON_H
