#include "Enemy.h"

#include "Character.h"
#include "MathUtils.h"

#include <cmath>

int Enemy::totalEnemiesCreated = 0;

Enemy::Enemy(const std::string& name, int hp, int level, int posX, int posY)
    : name(name), hp(hp), maxHp(hp), level(level), posX(posX), posY(posY) {
    ++totalEnemiesCreated;
}

void Enemy::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void Enemy::applyDepthScaling(int depth) {
    statMultiplier = 1.0 + 0.25 * depth; // +25% per floor below the first
    maxHp = static_cast<int>(std::lround(maxHp * statMultiplier));
    hp = maxHp;
}

int Enemy::attackDamage() const {
    return static_cast<int>(std::lround(computeAttackDamage() * statMultiplier));
}

bool Enemy::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
    return hp == 0;
}

void Enemy::healHp(int amount) {
    hp = util::clamp(hp + amount, 0, maxHp);
}


void Enemy::display(std::ostream& os) const {
    doDisplay(os);
}

std::ostream& operator<<(std::ostream& os, const Enemy& e) {
    e.display(os);
    return os;
}