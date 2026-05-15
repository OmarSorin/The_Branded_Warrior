#include "Enemy.h"

#include "Character.h"

int Enemy::totalEnemiesCreated = 0;

Enemy::Enemy(const std::string& name, int hp, int level, int posX, int posY)
    : name(name), hp(hp), maxHp(hp), level(level), posX(posX), posY(posY) {
    ++totalEnemiesCreated;
}

void Enemy::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

bool Enemy::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
    return hp == 0;
}

void Enemy::healHp(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}


void Enemy::display(std::ostream& os) const {
    doDisplay(os);
}

std::ostream& operator<<(std::ostream& os, const Enemy& e) {
    e.display(os);
    return os;
}
