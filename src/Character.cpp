#include "Character.h"

int Character::xpForNextLevel() const { return level * 50 + 50; }

int Character::checkLevelUp() {
  int levelsGained = 0;
  while (xp >= xpForNextLevel()) {
    xp -= xpForNextLevel();
    ++level;
    int hpBoost = 10 + level * 2;
    maxHp += hpBoost;
    hp += hpBoost;
    ++levelsGained;
  }
  return levelsGained;
}

Character::Character(const std::string &name, int hp, int level,
                     const Weapon &weapon, const Inventory &inv, int posX,
                     int posY)
    : name(name), hp(hp), maxHp(hp), level(level), xp(0), inventory(inv),
      posX(posX), posY(posY) {
  // Arma initiala merge pe prima pozitie in inventar
  inventory.addWeapon(weapon);
}

int Character::getXpForNextLevel() const { return xpForNextLevel(); }

void Character::setPosition(int x, int y) {
  posX = x;
  posY = y;
}

void Character::move(int dx, int dy) {
  posX += dx;
  posY += dy;
}

void Character::pickUpWeapon(const Weapon &w) { inventory.addWeapon(w); }

int Character::attackTarget(Character &target) {
  if (!isAlive()) {
    return 0;
  }

  int dealt = inventory.getWeapon(0).attack();
  if (dealt == 0) {
    return 0;
  }

  target.takeDamage(dealt);

  if (!target.isAlive()) {
    int xpGained = target.level * 30 + 20;
    gainXp(xpGained);
  }

  return dealt;
}

bool Character::takeDamage(int amount) {
  hp -= amount;
  if (hp < 0) {
    hp = 0;
  }
  return hp == 0;
}

int Character::heal(const Potion &potion) {
  if (!isAlive()) {
    return 0;
  }
  if (hp >= maxHp) {
    return 0;
  }
  int healed = potion.consume(hp, maxHp);
  hp += healed;
  return healed;
}

int Character::gainXp(int amount) {
  xp += amount;
  return checkLevelUp();
}

std::ostream &operator<<(std::ostream &os, const Character &c) {
  os << "=== " << c.name << " ===\n"
     << "  Level: " << c.level << " (XP: " << c.xp << "/"
     << c.xpForNextLevel() << ")\n"
     << "  HP: " << c.hp << "/" << c.maxHp << "\n"
     << "  Pos: (" << c.posX << ", " << c.posY << ")\n"
     << "  " << c.inventory;
  return os;
}
