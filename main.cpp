#include <iostream>
#include <array>
#include <chrono>
#include <random>
#include <thread>
#include <string>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "include/Example.h"


class Weapon {
private:
    std::string name;
    int damage;
    int durability;
    int maxDurability;

    // Funcție privată: calculează dacă atacul este critic (20% șansă, x2 damage)
    int rollCritical() const {
        static std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dist(1, 5);
        if (dist(rng) == 1) {
            return damage * 2;
        }
        return damage;
    }

public:
    // Constructor cu parametri
    Weapon(const std::string& name = "Fists", int damage = 2, int durability = 100)
        : name(name), damage(damage), durability(durability), maxDurability(durability) {}

    // Getters (const)
    const std::string& getName() const { return name; }
    int getDamage() const { return damage; }
    int getDurability() const { return durability; }
    int getMaxDurability() const { return maxDurability; }

    bool isBroken() const {
        return durability <= 0;
    }

    // Funcție non-trivială: atacă și reduce durabilitatea
    int attack() {
        if (isBroken()) {
            return 0;
        }
        int dealt = rollCritical();
        --durability;
        if (durability < 0) { durability = 0; }
        return dealt;
    }

    // Funcție: repară arma (nu depășește maxDurability)
    void repair(int amount) {
        durability += amount;
        if (durability > maxDurability) {
            durability = maxDurability;
        }
    }

    // operator<< pentru afișare

};
  std::ostream &operator<<(std::ostream &os, const Weapon &w) {
    os << "Weapon[" << w.getName() << ", dmg=" << w.getDamage()
       << ", dur=" << w.getDurability() << "/" << w.getMaxDurability();
    if (w.isBroken()) {
      os << " (BROKEN)";
    }
    os << "]";
    return os;
  }

class Potion {
private:
    std::string name;
    int healAmount;
    int price;

    int rollOverheal() const {
      static std::mt19937 rng{std::random_device{}()};
      std::uniform_int_distribution<int> coinFlip(0, 1);
      if (coinFlip(rng) == 1) {
        std::uniform_int_distribution<int> bonusDist(3, 5);
        return bonusDist(rng);
      }
      return 0;
    }

public:
    // Constructor cu parametri
    Potion(const std::string& name = "Unknown Potion", int healAmount = 10, int price = 5)
        : name(name), healAmount(healAmount), price(price) {}

    // Getters (const)
    const std::string& getName() const { return name; }
    int getHealAmount() const { return healAmount; }
    int getPrice() const { return price; }

    // Funcție non-trivială: compară eficiența poțiunilor (vindecamant per unitate de preț)
    bool isStrongerThan(const Potion& other) const {
        if (price == 0 && other.price == 0) {
            return healAmount > other.healAmount;
        }
        if (other.price == 0) return false;
        if (price == 0) return true;
        double thisRatio = static_cast<double>(healAmount) / price;
        double otherRatio = static_cast<double>(other.healAmount) / other.price;
        return thisRatio > otherRatio;
    }

    // Funcție: calculează vindecarea efectivă (ține cont de HP maxim)
    int consume(int currentHp, int maxHp) const {
      int effectiveHeal = healAmount;
        if (currentHp + effectiveHeal > maxHp) {
          effectiveHeal = maxHp - currentHp;
        }
      effectiveHeal += rollOverheal();
      return effectiveHeal;
    }

};
  std::ostream &operator<<(std::ostream &os, const Potion &p) {
    os << "Potion[" << p.getName() << ", heal=" << p.getHealAmount()
      << ", price=" << p.getPrice() << "]";
    return os;
  }

class Inventory {
private:
  Weapon *weapons;
  int size;
  int capacity;

  // Funcție privată: redimensionează array-ul intern
  void resize(int newCapacity) {
    Weapon *newWeapons = new Weapon[newCapacity];
    for (int i = 0; i < size; ++i) {
      newWeapons[i] = weapons[i];
    }
    delete[] weapons;
    weapons = newWeapons;
    capacity = newCapacity;
  }

public:
  // Constructor cu parametri
  explicit Inventory(int capacity = 5)
      : weapons(new Weapon[capacity]), size(0), capacity(capacity) {}

  // --- Rule of Three ---

  // Constructor de copiere (deep copy)
  Inventory(const Inventory &other)
      : weapons(new Weapon[other.capacity]), size(other.size),
        capacity(other.capacity) {
    for (int i = 0; i < size; ++i) {
      weapons[i] = other.weapons[i];
    }
  }

  // Operator= de copiere (copy and swap)
  Inventory &operator=(Inventory other) {
    this->swap(other);
    return *this;
  }

  // Destructor
  ~Inventory() { delete[] weapons; }

  // Swap (folosit de copy-and-swap)
  void swap(Inventory &other) noexcept {
    std::swap(weapons, other.weapons);
    std::swap(size, other.size);
    std::swap(capacity, other.capacity);
  }

  // --- Funcții publice ---

  int getSize() const { return size; }
  int getCapacity() const { return capacity; }
  bool isEmpty() const { return size == 0; }
  bool isFull() const { return size >= capacity; }

  // Adaugă o armă în inventar (redimensionare automată)
  bool addWeapon(const Weapon &w) {
    if (size >= capacity) {
      resize(capacity * 2);
    }
    weapons[size] = w;
    ++size;
    return true;
  }

  // Elimină o armă din inventar (prin index)
  bool removeWeapon(int index) {
    if (index < 0 || index >= size) {
      return false;
    }
    for (int i = index; i < size - 1; ++i) {
      weapons[i] = weapons[i + 1];
    }
    --size;
    return true;
  }

  // Acces la o armă (const)
  const Weapon &getWeapon(int index) const { return weapons[index]; }

  // Acces la o armă (non-const, necesar pentru attack care modifică durability)
  Weapon &getWeapon(int index) { return weapons[index]; }

  // Funcție non-trivială: găsește indexul armei cea mai puternică (care nu e
  // stricată)
  int findStrongestIndex() const {
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

  // Funcție non-trivială: calculează damage-ul total al armelor funcționale
  int totalDamage() const {
    int total = 0;
    for (int i = 0; i < size; ++i) {
      if (!weapons[i].isBroken()) {
        total += weapons[i].getDamage();
      }
    }
    return total;
  }

  // Funcție non-trivială: elimină toate armele stricate din inventar
  int removeAllBroken() {
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

};
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

  // Funcție privată: calculează XP-ul necesar pentru următorul nivel
  int xpForNextLevel() const { return level * 50 + 50; }

  // Funcție privată: verifică și aplică level up, returnează nr. de niveluri
  // câștigate
  int checkLevelUp() {
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

public:
  // Constructor cu parametri
  // Arma primită este adăugată ca slot 0 în inventar (= arma echipată)
  Character(const std::string &name = "Hero", int hp = 100, int level = 1,
            const Weapon &weapon = Weapon(), const Inventory &inv = Inventory(),
            int posX = 0, int posY = 0)
      : name(name), hp(hp), maxHp(hp), level(level), xp(0), inventory(inv),
        posX(posX), posY(posY) {
    // Arma inițială merge pe prima poziție în inventar
    inventory.addWeapon(weapon);
  }

  // Getters (const)
  const std::string &getName() const { return name; }
  int getHp() const { return hp; }
  int getMaxHp() const { return maxHp; }
  int getLevel() const { return level; }
  int getXp() const { return xp; }
  int getXpForNextLevel() const { return xpForNextLevel(); }
  bool isAlive() const { return hp > 0; }

  // Arma echipată = prima armă din inventar (slot 0)
  const Weapon &getEquippedWeapon() const { return inventory.getWeapon(0); }

  const Inventory &getInventory() const { return inventory; }

  // Getteri și setteri pentru coordonate
  int getX() const { return posX; }
  int getY() const { return posY; }
  void setPosition(int x, int y) {
    posX = x;
    posY = y;
  }
  void move(int dx, int dy) {
    posX += dx;
    posY += dy;
  }

  // Adaugă armă în inventar
  void pickUpWeapon(const Weapon &w) { inventory.addWeapon(w); }

  // --- Funcții non-triviale ---

  // Atacă un alt personaj. Returnează damage-ul dat (0 dacă atacul a eșuat).
  int attackTarget(Character &target) {
    if (!isAlive()) {
      return 0;
    }

    // Atacă cu arma echipată (slot 0)
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

  // Primește damage, returnează true dacă personajul a murit
  bool takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) {
      hp = 0;
    }
    return hp == 0;
  }

  // Folosește o poțiune, returnează câte HP s-au vindecat (0 dacă nu s-a
  // vindecat)
  int heal(const Potion &potion) {
    if (!isAlive()) {
      return 0;
    }
    int healed = potion.consume(hp, maxHp);
    if (healed <= 0) {
      return 0;
    }
    hp += healed;
    return healed;
  }

  // Câștigă XP și verifică level up, returnează nr. de niveluri câștigate
  int gainXp(int amount) {
    xp += amount;
    return checkLevelUp();
  }

};
  std::ostream &operator<<(std::ostream &os, const Character &c) {
    os << "=== " << c.getName() << " ===\n"
       << "  Level: " << c.getLevel() << " (XP: " << c.getXp() << "/"
       << c.getXpForNextLevel() << ")\n"
       << "  HP: " << c.getHp() << "/" << c.getMaxHp() << "\n"
       << "  Pos: (" << c.getX() << ", " << c.getY() << ")\n"
       << "  " << c.getInventory();
    return os;
  }


int main() {

  std::cout << "========================================\n";
  std::cout << "   ROGUELIKE RPG - Scenariu de Joc\n";
  std::cout << "========================================\n\n";

  // --- 1. Creăm arme ---
  std::cout << "--- Creare Arme ---\n";
  Weapon sword("Iron Sword", 15, 10);
  Weapon axe("Battle Axe", 20, 8);
  Weapon dagger("Shadow Dagger", 12, 15);
  Weapon hammer("War Hammer", 25, 5);

  std::cout << sword << "\n";
  std::cout << axe << "\n";
  std::cout << dagger << "\n";
  std::cout << hammer << "\n\n";

  // --- 2. Creăm poțiuni ---
  std::cout << "--- Creare Potiuni ---\n";
  Potion smallPotion("Small Heal", 20, 10);
  Potion bigPotion("Greater Heal", 50, 30);
  Potion freePotion("Free Sample", 5, 0);

  std::cout << smallPotion << "\n";
  std::cout << bigPotion << "\n";
  std::cout << freePotion << "\n";

  // Comparăm poțiuni (isStrongerThan)
  std::cout << "\n"
            << smallPotion.getName() << " vs " << bigPotion.getName() << ": ";
  if (smallPotion.isStrongerThan(bigPotion)) {
    std::cout << smallPotion.getName() << " are un raport heal/pret mai bun.\n";
  } else {
    std::cout << bigPotion.getName() << " are un raport heal/pret mai bun.\n";
  }
  std::cout << "\n";

  // --- 3. Creăm inventar și demonstrăm funcționalități ---
  std::cout << "--- Creare Inventar ---\n";
  Inventory inv(3);
  inv.addWeapon(sword);
  inv.addWeapon(dagger);
  inv.addWeapon(axe);
  std::cout << inv << "\n\n";

  // findStrongestIndex
  int bestIdx = inv.findStrongestIndex();
  if (bestIdx != -1) {
    std::cout << "Cea mai puternica arma: " << inv.getWeapon(bestIdx) << "\n";
  }

  // totalDamage
  std::cout << "Damage total in inventar: " << inv.totalDamage() << "\n\n";

  // --- 4. Demonstrăm Rule of Three ---
  std::cout << "--- Rule of Three: Copy Constructor ---\n";
  Inventory invCopy(inv);
  std::cout << "Original: " << inv << "\n";
  std::cout << "Copie:    " << invCopy << "\n";

  // Modificăm copia si verificăm că originalul NU se schimbă (deep copy)
  invCopy.removeWeapon(0);
  std::cout << "\nDupa stergerea primei arme din copie:\n";
  std::cout << "Original: " << inv << "\n";
  std::cout << "Copie:    " << invCopy << "\n\n";

  std::cout << "--- Rule of Three: Operator= ---\n";
  Inventory invAssigned;
  invAssigned = inv;
  std::cout << "Atribuit: " << invAssigned << "\n\n";

  // --- 5. Creăm personaje ---
  // Arma echipată = prima armă din inventar (slot 0)
  std::cout << "--- Creare Personaje ---\n";
  Character hero("Aldric", 120, 1, sword);
  Character enemy("Goblin", 40, 1, dagger);

  hero.pickUpWeapon(axe);
  hero.pickUpWeapon(hammer);

  std::cout << hero << "\n\n";
  std::cout << enemy << "\n\n";

  // --- 6. Scenariu de luptă ---
  std::cout << "========================================\n";
  std::cout << "   LUPTA: " << hero.getName() << " vs " << enemy.getName()
            << "\n";
  std::cout << "========================================\n\n";

  int round = 1;
  while (hero.isAlive() && enemy.isAlive()) {
    std::cout << "--- Runda " << round << " ---\n";

    // Salvăm starea înainte de atac
    bool heroWeaponBroken = hero.getEquippedWeapon().isBroken();
    int heroBaseDmg = hero.getEquippedWeapon().getDamage();

    int heroDmg = hero.attackTarget(enemy);

    if (!hero.isAlive()) {
      std::cout << "  " << hero.getName()
                << " nu mai este in viata si nu poate ataca!\n";
    } else if (heroDmg == 0 && heroWeaponBroken) {
      std::cout << "  " << hero.getName() << " incearca sa atace, dar arma ("
                << hero.getEquippedWeapon().getName() << ") este stricata!\n";
    } else if (heroDmg > 0) {
      if (heroDmg > heroBaseDmg) {
        std::cout << "  ** CRITICAL HIT! **\n";
      }
      std::cout << "  " << hero.getName() << " ataca pe " << enemy.getName()
                << " cu " << hero.getEquippedWeapon().getName() << " pentru "
                << heroDmg << " damage!\n";
      if (!enemy.isAlive()) {
        int xpGained = 1 * 30 + 20; // enemy level * 30 + 20
        std::cout << "  " << enemy.getName() << " a cazut in lupta! (HP: 0)\n";
        std::cout << "  " << enemy.getName() << " a fost invins! "
                  << hero.getName() << " castiga " << xpGained << " XP!\n";
      } else {
        std::cout << "  " << enemy.getName() << " are acum " << enemy.getHp()
                  << "/" << enemy.getMaxHp() << " HP.\n";
      }
    }

    // Inamicul atacă (dacă e în viață)
    if (enemy.isAlive()) {
      bool enemyWeaponBroken = enemy.getEquippedWeapon().isBroken();
      int enemyBaseDmg = enemy.getEquippedWeapon().getDamage();

      int enemyDmg = enemy.attackTarget(hero);

      if (!enemy.isAlive()) {
        std::cout << "  " << enemy.getName()
                  << " nu mai este in viata si nu poate ataca!\n";
      } else if (enemyDmg == 0 && enemyWeaponBroken) {
        std::cout << "  " << enemy.getName() << " incearca sa atace, dar arma ("
                  << enemy.getEquippedWeapon().getName()
                  << ") este stricata!\n";
      } else if (enemyDmg > 0) {
        if (enemyDmg > enemyBaseDmg) {
          std::cout << "  ** CRITICAL HIT! **\n";
        }
        std::cout << "  " << enemy.getName() << " ataca pe " << hero.getName()
                  << " cu " << enemy.getEquippedWeapon().getName() << " pentru "
                  << enemyDmg << " damage!\n";
        if (!hero.isAlive()) {
          int xpGained = 1 * 30 + 20;
          std::cout << "  " << hero.getName() << " a cazut in lupta! (HP: 0)\n";
          std::cout << "  " << hero.getName() << " a fost invins! "
                    << enemy.getName() << " castiga " << xpGained << " XP!\n";
        } else {
          std::cout << "  " << hero.getName() << " are acum " << hero.getHp()
                    << "/" << hero.getMaxHp() << " HP.\n";
        }
      }
    }

    std::cout << "\n";
    ++round;
  }

  // --- 7. Vindecare cu poțiune ---
  if (hero.isAlive()) {
    std::cout << "--- Dupa Lupta: Vindecare ---\n";
    std::cout << hero.getName() << " are " << hero.getHp() << "/"
              << hero.getMaxHp() << " HP.\n";

    int healed1 = hero.heal(bigPotion);
    if (healed1 > 0) {
      std::cout << "  " << hero.getName() << " foloseste "
                << bigPotion.getName() << " si se vindeca cu " << healed1
                << " HP. (" << hero.getHp() << "/" << hero.getMaxHp() << ")\n";
    } else {
      std::cout << "  " << hero.getName() << " are deja HP maxim!\n";
    }

    int healed2 = hero.heal(smallPotion);
    if (healed2 > 0) {
      std::cout << "  " << hero.getName() << " foloseste "
                << smallPotion.getName() << " si se vindeca cu " << healed2
                << " HP. (" << hero.getHp() << "/" << hero.getMaxHp() << ")\n";
    } else {
      std::cout << "  " << hero.getName() << " are deja HP maxim!\n";
    }

    std::cout << "\n" << hero << "\n\n";
  }

  // --- 8. Demonstrăm removeAllBroken ---
  std::cout << "--- Curatare Inventar ---\n";
  Weapon fragile("Glass Sword", 30, 1);
  Weapon tempFragile = fragile;
  tempFragile.attack(); // stricam arma (durability 1 -> 0)
  tempFragile.attack();

  Inventory cleanupInv;
  cleanupInv.addWeapon(tempFragile);
  cleanupInv.addWeapon(sword);
  cleanupInv.addWeapon(axe);
  std::cout << "Inainte: " << cleanupInv << "\n";
  int removed = cleanupInv.removeAllBroken();
  std::cout << "Arme stricate eliminate: " << removed << "\n";
  std::cout << "Dupa:    " << cleanupInv << "\n\n";

  // --- 9. Reparare armă ---
  std::cout << "--- Reparare Arma ---\n";
  Weapon damagedSword("Rusted Blade", 10, 3);
  std::cout << "Inainte: " << damagedSword << "\n";
  damagedSword.repair(50);
  std::cout << "Dupa reparare (+50): " << damagedSword << "\n\n";

  // --- 10. Citire de la tastatură ---
  std::cout << "--- Creare Personaj de la Tastatura ---\n";
  std::string playerName;
  int playerHp;
  std::string weaponName;
  int weaponDmg;

  std::cout << "Nume personaj: ";
  std::getline(std::cin, playerName);
  std::cout << "HP initial: ";
  std::cin >> playerHp;
  std::cin.ignore();
  std::cout << "Nume arma: ";
  std::getline(std::cin, weaponName);
  std::cout << "Damage arma: ";
  std::cin >> weaponDmg;

  Weapon customWeapon(weaponName, weaponDmg, 20);
  Character customPlayer(playerName, playerHp, 1, customWeapon);
  std::cout << "\nPersonajul tau:\n" << customPlayer << "\n";

  std::cout << "\n========================================\n";
  std::cout << "   Sfarsit Scenariu\n";
  std::cout << "========================================\n";

  return 0;
}

