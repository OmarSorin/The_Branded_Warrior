#include "Hobbit.h"
#include "Character.h"
#include "Map.h"
#include <cstdlib>

int Hobbit::totalLuckyHits = 0;

Hobbit::Hobbit(const std::string& name, int posX, int posY)
    : Enemy(name, 50, 2, posX, posY) {}

std::unique_ptr<Enemy> Hobbit::clone() const {
    return std::make_unique<Hobbit>(*this);
}

int Hobbit::computeAttackDamage() const {
    // 20% chance for lucky hit
    if (std::rand() % 100 < 20) {
        ++totalLuckyHits;
        return 75;
    }
    return 15;
}

void Hobbit::onDeath(Character& killer) {
    killer.gainXp(50);
}

void Hobbit::doDisplay(std::ostream& os) const {
    os << "Hobbit [HP: " << getHp() << "/" << getMaxHp()
       << ", Lucky hits landed: " << totalLuckyHits << "]";
}

void Hobbit::updateAI(Character& hero, const Map& dungeon) {
    int dx = hero.getX() - getX();
    int dy = hero.getY() - getY();
    int dist = std::abs(dx) + std::abs(dy);

    // Attack if adjacent
    if (dist == 1) {
        hero.takeDamage(computeAttackDamage());
        return;
    }

    // Move closer using simple step
    int stepX = getX() + (dx != 0 ? (dx > 0 ? 1 : -1) : 0);
    int stepY = getY() + (dy != 0 ? (dy > 0 ? 1 : -1) : 0);
    if (dungeon.isWalkable(stepX, getY()))
        setPosition(stepX, getY());
    else if (dungeon.isWalkable(getX(), stepY))
        setPosition(getX(), stepY);
}

void Hobbit::applyDrops(EnemyManager& manager) {
    manager.modifyPotions("medium", 1);
}