#include "GameDemo.h"

#include <array>
#include <iostream>
#include <string>

void GameDemo::demoWeapons() {
  std::cout << "--- Creare Arme ---\n";
  std::cout << sword << "\n";
  std::cout << axe << "\n";
  std::cout << dagger << "\n";
  std::cout << hammer << "\n\n";
}

void GameDemo::demoPotions() {
  std::cout << "--- Creare Potiuni ---\n";
  std::cout << smallPotion << "\n";
  std::cout << bigPotion << "\n";
  std::cout << freePotion << "\n";

  std::cout << "\n"
            << smallPotion.getName() << " vs " << bigPotion.getName() << ": ";
  if (smallPotion.isStrongerThan(bigPotion)) {
    std::cout << smallPotion.getName()
              << " are un raport heal/pret mai bun.\n";
  } else {
    std::cout << bigPotion.getName() << " are un raport heal/pret mai bun.\n";
  }
  std::cout << "\n";
}

void GameDemo::demoInventory() {
  std::cout << "--- Creare Inventar ---\n";
  Inventory inv(3);
  inv.addWeapon(sword);
  inv.addWeapon(dagger);
  inv.addWeapon(axe);
  std::cout << inv << "\n\n";

  int bestIdx = inv.findStrongestIndex();
  if (bestIdx != -1) {
    std::cout << "Cea mai puternica arma: " << inv.getWeapon(bestIdx) << "\n";
  }
  std::cout << "Damage total in inventar: " << inv.totalDamage() << "\n\n";
}

void GameDemo::demoRuleOfThree() {
  std::cout << "--- Copy Constructor ---\n";
  Inventory inv(3);
  inv.addWeapon(sword);
  inv.addWeapon(dagger);
  inv.addWeapon(axe);

  Inventory invCopy(inv);
  std::cout << "Original: " << inv << "\n";
  std::cout << "Copie:    " << invCopy << "\n";

  invCopy.removeWeapon(0);
  std::cout << "\nDupa stergerea primei arme din copie:\n";
  std::cout << "Original: " << inv << "\n";
  std::cout << "Copie:    " << invCopy << "\n\n";

  std::cout << "--- Rule of Three: Operator= ---\n";
  Inventory invAssigned;
  invAssigned = inv;
  std::cout << "Atribuit: " << invAssigned << "\n\n";
}

void GameDemo::demoCharacters() {
  std::cout << "--- Creare Personaje ---\n";
  hero.pickUpWeapon(axe);
  hero.pickUpWeapon(hammer);
  std::cout << hero << "\n\n";
  std::cout << enemy << "\n\n";
}

void GameDemo::demoCombat() {
  std::cout << "========================================\n";
  std::cout << "   LUPTA: " << hero.getName() << " vs " << enemy.getName()
            << "\n";
  std::cout << "========================================\n\n";

  int round = 1;
  while (hero.isAlive() && enemy.isAlive()) {
    std::cout << "--- Runda " << round << " ---\n";

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
        int xpGained = 1 * 30 + 20;
        std::cout << "  " << enemy.getName()
                  << " a cazut in lupta! (HP: 0)\n";
        std::cout << "  " << enemy.getName() << " a fost invins! "
                  << hero.getName() << " castiga " << xpGained << " XP!\n";
      } else {
        std::cout << "  " << enemy.getName() << " are acum " << enemy.getHp()
                  << "/" << enemy.getMaxHp() << " HP.\n";
      }
    }

    if (enemy.isAlive()) {
      bool enemyWeaponBroken = enemy.getEquippedWeapon().isBroken();
      int enemyBaseDmg = enemy.getEquippedWeapon().getDamage();
      int enemyDmg = enemy.attackTarget(hero);

      if (!enemy.isAlive()) {
        std::cout << "  " << enemy.getName()
                  << " nu mai este in viata si nu poate ataca!\n";
      } else if (enemyDmg == 0 && enemyWeaponBroken) {
        std::cout << "  " << enemy.getName()
                  << " incearca sa atace, dar arma ("
                  << enemy.getEquippedWeapon().getName()
                  << ") este stricata!\n";
      } else if (enemyDmg > 0) {
        if (enemyDmg > enemyBaseDmg) {
          std::cout << "  ** CRITICAL HIT! **\n";
        }
        std::cout << "  " << enemy.getName() << " ataca pe " << hero.getName()
                  << " cu " << enemy.getEquippedWeapon().getName()
                  << " pentru " << enemyDmg << " damage!\n";
        if (!hero.isAlive()) {
          int xpGained = 1 * 30 + 20;
          std::cout << "  " << hero.getName()
                    << " a cazut in lupta! (HP: 0)\n";
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
}

void GameDemo::demoHealing() {
  if (!hero.isAlive())
    return;

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

void GameDemo::demoRemoveBroken() {
  std::cout << "--- Curatare Inventar ---\n";
  Weapon fragile("Glass Sword", 30, 1);
  Weapon tempFragile = fragile;
  tempFragile.attack();
  tempFragile.attack();

  Inventory cleanupInv;
  cleanupInv.addWeapon(tempFragile);
  cleanupInv.addWeapon(sword);
  cleanupInv.addWeapon(axe);
  std::cout << "Inainte: " << cleanupInv << "\n";
  int removed = cleanupInv.removeAllBroken();
  std::cout << "Arme stricate eliminate: " << removed << "\n";
  std::cout << "Dupa:    " << cleanupInv << "\n\n";
}

void GameDemo::demoRepair() {
  std::cout << "--- Reparare Arma ---\n";
  Weapon damagedSword("Rusted Blade", 10, 3);
  std::cout << "Inainte: " << damagedSword << "\n";
  damagedSword.repair(50);
  std::cout << "Dupa reparare (+50): " << damagedSword << "\n\n";
}

void GameDemo::demoKeyboardInput() {
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
}

void GameDemo::demoMovement() {
  std::cout << "\n========================================\n";
  std::cout << "   SIMULARE MISCARE\n";
  std::cout << "========================================\n\n";

  std::cout << hero.getName() << " porneste de la pozitia (" << hero.getX()
            << ", " << hero.getY() << ")\n\n";

  std::array<std::pair<int, int>, 8> moves = {
      {{1, 0}, {1, 0}, {1, 0}, {0, -1}, {0, -1}, {-1, 0}, {-1, 0}, {0, 1}}};

  for (size_t i = 0; i < moves.size(); ++i) {
    int dx = moves[i].first;
    int dy = moves[i].second;

    std::string direction;
    if (dx > 0)
      direction = "dreapta";
    else if (dx < 0)
      direction = "stanga";
    else if (dy > 0)
      direction = "jos";
    else
      direction = "sus";

    hero.move(dx, dy);
    std::cout << "  Pasul " << (i + 1) << ": se misca " << direction
              << " -> (" << hero.getX() << ", " << hero.getY() << ")\n";
  }

  std::cout << "\n"
            << hero.getName() << " a ajuns la (" << hero.getX() << ", "
            << hero.getY() << ")\n";

  hero.setPosition(10, 5);
  std::cout << hero.getName() << " se teleporteaza la (" << hero.getX()
            << ", " << hero.getY() << ")\n";

  std::cout << "\n" << hero << "\n";
}

void GameDemo::run() {
  std::cout << "========================================\n";
  std::cout << "   ROGUELIKE RPG - Scenariu de Joc\n";
  std::cout << "========================================\n\n";

  demoWeapons();
  demoPotions();
  demoInventory();
  demoRuleOfThree();
  demoCharacters();
  demoCombat();
  demoHealing();
  demoRemoveBroken();
  demoRepair();
  demoKeyboardInput();
  demoMovement();

  std::cout << "\n========================================\n";
  std::cout << "   Sfarsit Scenariu\n";
  std::cout << "========================================\n";
}
