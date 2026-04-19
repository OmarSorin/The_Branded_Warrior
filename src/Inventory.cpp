#include "Inventory.h"

#include <algorithm>

Inventory::Inventory(int capacity) {
  weapons.reserve(capacity);
}

bool Inventory::addWeapon(const Weapon &w) {
  weapons.push_back(w);
  return true;
}

bool Inventory::removeWeapon(int index) {
  if (index < 0 || index >= static_cast<int>(weapons.size())) {
    return false;
  }
  weapons.erase(weapons.begin() + index);
  return true;
}

int Inventory::findStrongestIndex() const {
  int bestIndex = -1;
  int bestDamage = -1;
  for (int i = 0; i < static_cast<int>(weapons.size()); ++i) {
    if (!weapons[i].isBroken() && weapons[i].getDamage() > bestDamage) {
      bestDamage = weapons[i].getDamage();
      bestIndex = i;
    }
  }
  return bestIndex;
}

int Inventory::totalDamage() const {
  int total = 0;
  for (const auto &w : weapons) {
    if (!w.isBroken()) {
      total += w.getDamage();
    }
  }
  return total;
}

int Inventory::removeAllBroken() {
  int before = static_cast<int>(weapons.size());
  auto newEnd = std::remove_if(weapons.begin(), weapons.end(),
                               [](const Weapon &w) { return w.isBroken(); });
  weapons.erase(newEnd, weapons.end());
  return before - static_cast<int>(weapons.size());
}

std::ostream &operator<<(std::ostream &os, const Inventory &inv) {
  int sz = static_cast<int>(inv.weapons.size());
  os << "Inventory[" << sz << "/" << static_cast<int>(inv.weapons.capacity()) << "]";
  if (sz > 0) {
    os << " {";
    for (int i = 0; i < sz; ++i) {
      os << "\n    " << (i + 1) << ". " << inv.weapons[i];
    }
    os << "\n  }";
  } else {
    os << " {empty}";
  }
  return os;
}
