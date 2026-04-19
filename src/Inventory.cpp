#include "Inventory.h"

#include <algorithm>

void Inventory::resize(int newCapacity) {
  Weapon *newWeapons = new Weapon[newCapacity];
  for (int i = 0; i < size; ++i) {
    newWeapons[i] = weapons[i];
  }
  delete[] weapons;
  weapons = newWeapons;
  capacity = newCapacity;
}

Inventory::Inventory(int capacity)
    : weapons(new Weapon[capacity]), size(0), capacity(capacity) {}

Inventory::Inventory(const Inventory &other)
    : weapons(new Weapon[other.capacity]), size(other.size),
      capacity(other.capacity) {
  for (int i = 0; i < size; ++i) {
    weapons[i] = other.weapons[i];
  }
}

Inventory &Inventory::operator=(Inventory other) {
  this->swap(other);
  return *this;
}

Inventory::~Inventory() { delete[] weapons; }

void Inventory::swap(Inventory &other) noexcept {
  std::swap(weapons, other.weapons);
  std::swap(size, other.size);
  std::swap(capacity, other.capacity);
}

bool Inventory::addWeapon(const Weapon &w) {
  if (size >= capacity) {
    resize(capacity * 2);
  }
  weapons[size] = w;
  ++size;
  return true;
}

bool Inventory::removeWeapon(int index) {
  if (index < 0 || index >= size) {
    return false;
  }
  for (int i = index; i < size - 1; ++i) {
    weapons[i] = weapons[i + 1];
  }
  --size;
  return true;
}

int Inventory::findStrongestIndex() const {
  int bestIndex = -1;
  int bestDamage = -1;
  for (int i = 0; i < size; ++i) {
    if (!weapons[i].isBroken() && weapons[i].getDamage() > bestDamage) {
      bestDamage = weapons[i].getDamage();
      bestIndex = i;
    }
  }
  return bestIndex;
}

int Inventory::totalDamage() const {
  int total = 0;
  for (int i = 0; i < size; ++i) {
    if (!weapons[i].isBroken()) {
      total += weapons[i].getDamage();
    }
  }
  return total;
}

int Inventory::removeAllBroken() {
  int removed = 0;
  int writeIdx = 0;
  for (int readIdx = 0; readIdx < size; ++readIdx) {
    if (!weapons[readIdx].isBroken()) {
      if (writeIdx != readIdx) {
        weapons[writeIdx] = weapons[readIdx];
      }
      ++writeIdx;
    } else {
      ++removed;
    }
  }
  size = writeIdx;
  return removed;
}

std::ostream &operator<<(std::ostream &os, const Inventory &inv) {
  os << "Inventory[" << inv.getSize() << "/" << inv.getCapacity() << "]";
  if (inv.getSize() > 0) {
    os << " {";
    for (int i = 0; i < inv.getSize(); ++i) {
      os << "\n    " << (i + 1) << ". " << inv.getWeapon(i);
    }
    os << "\n  }";
  } else {
    os << " {empty}";
  }
  return os;
}
