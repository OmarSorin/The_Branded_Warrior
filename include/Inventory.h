#ifndef INVENTORY_H
#define INVENTORY_H

#include "Weapon.h"
#include <ostream>

class Inventory {
private:
  Weapon *weapons;
  int size;
  int capacity;

  void resize(int newCapacity);

public:
  explicit Inventory(int capacity = 5);

  Inventory(const Inventory &other);

  Inventory &operator=(Inventory other);

  ~Inventory();

  void swap(Inventory &other) noexcept;

  int getSize() const { return size; }
  int getCapacity() const { return capacity; }
  bool isEmpty() const { return size == 0; }
  bool isFull() const { return size >= capacity; }

  bool addWeapon(const Weapon &w);

  bool removeWeapon(int index);

  const Weapon &getWeapon(int index) const { return weapons[index]; }

  // Acces la o arma (non-const, necesar pentru attack care modifica durability
  Weapon &getWeapon(int index) { return weapons[index]; }

  // cauta arma principala( cea mai puternica)
  int findStrongestIndex() const;

  // total damage
  int totalDamage() const;

  // sterge toate armele stricate
  int removeAllBroken();
};

std::ostream &operator<<(std::ostream &os, const Inventory &inv);

#endif // INVENTORY_H
