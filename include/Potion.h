#ifndef POTION_H
#define POTION_H

#include <ostream>
#include <string>

class Potion {
private:
  std::string name;
  int healAmount;
  int price;

  // 50% sansa de lucky heal: +3-5 HP bonus
  static int rollLuckyHeal();

public:
  explicit Potion(const std::string &name = "Unknown Potion", int healAmount = 10,
           int price = 5);

  const std::string &getName() const { return name; }
  int getHealAmount() const { return healAmount; }
  int getPrice() const { return price; }

  bool isStrongerThan(const Potion &other) const;

  // Overheal: potiunea vindeca complet (poate depasi maxHp), dar nu poti folosi
  //           potiuni daca esti deja la sau peste maxHp
  // Lucky heal: 50% sansa de +3-5 HP bonus
  int consume(int currentHp, int maxHp) const;
};

std::ostream &operator<<(std::ostream &os, const Potion &p);

#endif // POTION_H
