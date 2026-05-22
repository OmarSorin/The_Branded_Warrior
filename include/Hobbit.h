#ifndef HOBBIT_H
#define HOBBIT_H

#include "Enemy.h"

class Hobbit : public Enemy {
private:
    static int totalLuckyHits;
    void doDisplay(std::ostream& os) const override;

public:
    explicit Hobbit(const std::string& name = "Hobbit", int posX = 0, int posY = 0);

    std::unique_ptr<Enemy> clone() const override;

    int computeAttackDamage() const override;
    void onDeath(Character& killer) override;
    void updateAI(Character& hero, const Map& dungeon) override;

    static int getTotalLuckyHits() { return totalLuckyHits; }
};

#endif // HOBBIT_H