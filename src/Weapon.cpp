#include "Weapon.h"

#include <random>

int Weapon::rollCritical() const {
  static std::mt19937 rng{std::random_device{}()};
  std::uniform_int_distribution<int> dist(1, 5);
  if (dist(rng) == 1) {
    return damage * 2;
  }
  return damage;
}

Weapon::Weapon(const std::string &name, int damage, int durability)
    : name(name), damage(damage), durability(durability),
      maxDurability(durability) {}

int Weapon::attack() {
  if (isBroken()) {
    return 0;
  }
  int dealt = rollCritical();
  --durability;
  if (durability < 0) {
    durability = 0;
  }
  return dealt;
}

void Weapon::repair(int amount) {
  durability += amount;
  if (durability > maxDurability) {
    durability = maxDurability;
  }
}

std::ostream &operator<<(std::ostream &os, const Weapon &w) {
  os << "Weapon[" << w.name << ", dmg=" << w.damage
     << ", dur=" << w.durability << "/" << w.maxDurability;
  if (w.durability <= 0) {
    os << " (BROKEN)";
  }
  os << "]";
  return os;
}
