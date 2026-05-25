#include "Troll.h"

#include "Character.h"
#include "Map.h"
#include "Pathfinding.h"
#include <cstdlib>
#include <ostream>
#include <random>

Troll::Troll(const std::string& name, int posX, int posY)
    : Enemy(name, 100, 3, posX, posY) {
    detectionRange = 8;
}

std::unique_ptr<Enemy> Troll::clone() const {
    return std::make_unique<Troll>(*this);
}

int Troll::computeAttackDamage() const {
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(18, 28);
    return dist(rng); // heavy but unpredictable
}

void Troll::onDeath(Character& killer) {
    killer.gainXp(100);
}

void Troll::regenerate() {
    if (!isAlive()) return;
    healHp(REGEN_AMOUNT);
}


void Troll::doDisplay(std::ostream& os) const {
    os << "Troll[" << getName()
       << ", HP=" << getHp() << "/" << getMaxHp()
       << ", dmg=~" << computeAttackDamage()
       << ", regen=" << REGEN_AMOUNT << "/turn"
       << ", pos=(" << getX() << "," << getY() << ")]";
}

void Troll::updateAI(Character& hero, const Map& dungeon) {
    if (!isAlive()) return;

    // Troll acts only every TURN_COOLDOWN player turns
    ++turnCounter;
    if (turnCounter < TURN_COOLDOWN) return;
    turnCounter = 0;

    // Regenerate HP each time it takes a turn
    regenerate();

    int dist = std::abs(hero.getX() - getX()) + std::abs(hero.getY() - getY());

    // Trolls sense by proximity — no line-of-sight check needed
    if (!aggro && dist <= detectionRange) {
        aggro = true;
    }

    if (!aggro) return;

    // Attack if adjacent
    if (dist == 1) {
        hero.takeDamage(computeAttackDamage());
        return;
    }

    // BFS toward player
    auto path = findPath(dungeon, getX(), getY(), hero.getX(), hero.getY());
    if (path.size() > 1) setPosition(path[1].first, path[1].second);
}

void Troll::applyDrops(EnemyManager& manager) {
    manager.modifyPotions("large", 1);
}