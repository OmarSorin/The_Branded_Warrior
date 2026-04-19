#include "Potion.h"

#include <random>

int Potion::rollLuckyHeal() const {
  static std::mt19937 rng{std::random_device{}()};
  std::uniform_int_distribution<int> coinFlip(0, 1);
  if (coinFlip(rng) == 1) {
    std::uniform_int_distribution<int> bonusDist(3, 5);
    return bonusDist(rng);
  }
  return 0;
}

Potion::Potion(const std::string &name, int healAmount, int price)
    : name(name), healAmount(healAmount), price(price) {}

bool Potion::isStrongerThan(const Potion &other) const {
  if (price == 0 && other.price == 0) {
    return healAmount > other.healAmount;
  }
  if (other.price == 0)
    return false;
  if (price == 0)
    return true;
  double thisRatio = static_cast<double>(healAmount) / price;
  double otherRatio = static_cast<double>(other.healAmount) / other.price;
  return thisRatio > otherRatio;
}

int Potion::consume(int currentHp, int maxHp) const {
  if (currentHp >= maxHp) {
    return 0;
  }
  return healAmount + rollLuckyHeal();
}

std::ostream &operator<<(std::ostream &os, const Potion &p) {
  os << "Potion[" << p.getName() << ", heal=" << p.getHealAmount()
     << ", price=" << p.getPrice() << "]";
  return os;
}
