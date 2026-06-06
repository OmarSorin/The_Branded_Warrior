#include "Goblin.h"

#include "Character.h"
#include "Map.h"
#include "Pathfinding.h"
#include <cstdlib>
#include <ostream>

Goblin::Goblin(const std::string& name, int posX, int posY)
    : Enemy(name, 30, 1, posX, posY) {}

std::unique_ptr<Enemy> Goblin::clone() const {
    return std::make_unique<Goblin>(*this);
}

int Goblin::computeAttackDamage() const {
    return 8; // low, flat damage — no variance
}

void Goblin::onDeath(Character& killer) {
    killer.gainXp(40);
}

void Goblin::doDisplay(std::ostream& os) const {
    os << "Goblin[" << getName()
       << ", HP=" << getHp() << "/" << getMaxHp()
       << ", dmg=" << computeAttackDamage()
       << ", pos=(" << getX() << "," << getY() << ")]";
}

void Goblin::updateAI(Character& hero, const Map& dungeon) {
    if (!isAlive()) return;

    int dist = std::abs(hero.getX() - getX()) + std::abs(hero.getY() - getY());

    // Detect player: needs LOS and within 6 tiles
    if (!aggro && dist <= detectionRange
        && dungeon.hasLineOfSight(getX(), getY(), hero.getX(), hero.getY())) {
        aggro = true;
    }

    if (!aggro) return;

    // Attack if adjacent
    if (dist == 1) {
        hero.takeDamage(computeAttackDamage());
        return;
    }

    // BFS toward player, take one step
    auto path = findPath(dungeon, getX(), getY(), hero.getX(), hero.getY());
    if (path.size() > 1) {
        setPosition(path[1].first, path[1].second);
    }
}

void Goblin::applyDrops(EnemyManager& manager) {
    manager.modifyPotions("small", 1);
}

const sf::Texture* Goblin::getTexture() const {
    static sf::Texture tex = []() {
        sf::Texture t;
        (void)t.loadFromFile("assets/goblin.png");
        return t;
    }();
    return &tex;
}

