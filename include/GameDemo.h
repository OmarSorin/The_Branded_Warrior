#ifndef GAMEDEMO_H
#define GAMEDEMO_H

#include "Character.h"
#include "Inventory.h"
#include "Potion.h"
#include "Weapon.h"

class GameDemo {
private:
  Weapon sword{"Iron Sword", 15, 10};
  Weapon axe{"Battle Axe", 20, 8};
  Weapon dagger{"Shadow Dagger", 12, 15};
  Weapon hammer{"War Hammer", 25, 5};

  Potion smallPotion{"Small Heal", 20, 10};
  Potion bigPotion{"Greater Heal", 50, 30};
  Potion freePotion{"Free Sample", 5, 0};

  Character hero{"Aldric", 120, 1, sword};
  Character enemy{"Goblin", 40, 1, dagger};

  void demoWeapons();
  void demoPotions();
  void demoInventory();
  void demoRuleOfThree();
  void demoCharacters();
  void demoCombat();
  void demoHealing();
  void demoRemoveBroken();
  static void demoRepair();
  static void demoKeyboardInput();
  void demoMovement();

public:
  // Ruleaza toate demo-urile in ordine
  // cppcheck-suppress unusedFunction
  void run();
};

#endif // GAMEDEMO_H
