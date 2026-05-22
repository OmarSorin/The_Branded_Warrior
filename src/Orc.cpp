#include "Orc.h"

#include "Character.h"
#include "Map.h"
#include "Pathfinding.h"
#include <cstdlib>
#include <ostream>

Orc::Orc(const std::string& name, int posX, int posY)
    : Enemy(name, 60, 2, posX, posY) {
    detectionRange = 5;
}

std::unique_ptr<Enemy> Orc::clone() const {
    return std::make_unique<Orc>(*this);
}

int Orc::computeAttackDamage() const {
    const int baseDamage = 15;
    // Rage: below 30% HP, damage doubles
    if (getHp() * 10 < getMaxHp() * 3) {
        return baseDamage * 2;
    }
    return baseDamage;
}

void Orc::onDeath(Character& killer) {
    killer.gainXp(70);
}

void Orc::doDisplay(std::ostream& os) const {
    const bool enraged = getHp() * 10 < getMaxHp() * 3;
    os << "Orc[" << getName()
       << ", HP=" << getHp() << "/" << getMaxHp()
       << ", dmg=" << computeAttackDamage()
       << (enraged ? " (ENRAGED)" : "")
       << ", pos=(" << getX() << "," << getY() << ")]";
}

void Orc::updateAI(Character& hero, const Map& dungeon) {
    if (!isAlive()) return;

    const bool enraged = getHp() * 10 < getMaxHp() * 3;
    // Enraged orcs have a much larger detection radius
    const int range = enraged ? 9 : detectionRange;

    int dist = std::abs(hero.getX() - getX()) + std::abs(hero.getY() - getY());

    // Detect: needs LOS
    if (!aggro && dist <= range
        && dungeon.hasLineOfSight(getX(), getY(), hero.getX(), hero.getY())) {
        aggro = true;
    }

    if (!aggro) return;

    // Attack if adjacent
    if (dist == 1) {
        hero.takeDamage(computeAttackDamage());
        return;
    }

    if (enraged) {
        // Greedy charge: step in whichever of the 4 directions closes the distance most
        const int dx[] = { 0,  0,  1, -1 };
        const int dy[] = { 1, -1,  0,  0 };
        int bestX = getX(), bestY = getY(), bestDist = dist;
        for (int i = 0; i < 4; ++i) {
            int nx = getX() + dx[i];
            int ny = getY() + dy[i];
            bool isHeroTile = (nx == hero.getX() && ny == hero.getY());
            if (!isHeroTile && !dungeon.isWalkable(nx, ny)) continue;
            int d = std::abs(hero.getX() - nx) + std::abs(hero.getY() - ny);
            if (d < bestDist) { bestDist = d; bestX = nx; bestY = ny; }
        }
        if (bestX != getX() || bestY != getY()) setPosition(bestX, bestY);
    } else {
        // BFS
        auto path = findPath(dungeon, getX(), getY(), hero.getX(), hero.getY());
        if (path.size() > 1) setPosition(path[1].first, path[1].second);
    }
}
