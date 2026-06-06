#ifndef HOBBIT_H
#define HOBBIT_H

#include "Enemy.h"
#include "EnemyManager.h"

class EnemyManager; //forward declaration


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
    bool hasLandedLuckyHit() const { return totalLuckyHits > 0; }

    //apply drops
    void applyDrops(EnemyManager& manager) override;

    const sf::Texture* getTexture() const override;
};

#endif // HOBBIT_H