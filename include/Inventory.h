#ifndef INVENTORY_H
#define INVENTORY_H

#include "Weapon.h"
#include <ostream>
#include <vector>

class Inventory {
private:
  std::vector<Weapon> weapons;

public:
  // Reserve space upfront; vector grows automatically if needed
  explicit Inventory(int capacity = 5);

  // Rule of Zero: std::vector handles copy, move, and destruction automatically

  // cppcheck-suppress unusedFunction
  int getSize() const { return static_cast<int>(weapons.size()); }
  // cppcheck-suppress unusedFunction
  int getCapacity() const { return static_cast<int>(weapons.capacity()); }
  // cppcheck-suppress unusedFunction
  bool isEmpty() const { return weapons.empty(); }
  // cppcheck-suppress unusedFunction
  bool isFull() const { return false; } // vector grows automatically

  bool addWeapon(const Weapon &w);

  bool removeWeapon(int index);

  const Weapon &getWeapon(int index) const { return weapons[index]; }

  // Acces la o arma (non-const, necesar pentru attack care modifica durability)
  Weapon &getWeapon(int index) { return weapons[index]; }

  // cauta arma principala (cea mai puternica)
  int findStrongestIndex() const;

  // total damage
  int totalDamage() const;

  // sterge toate armele stricate
  int removeAllBroken();

  friend std::ostream &operator<<(std::ostream &os, const Inventory &inv);
};

std::ostream &operator<<(std::ostream &os, const Inventory &inv);

#endif // INVENTORY_H
