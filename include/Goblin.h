#ifndef GOBLIN_H
#define GOBLIN_H

#include "Enemy.h"
#include "EnemyManager.h"

class EnemyManager; //forward declaration

// Goblin: weak but consistent — low HP, reliable damage output
class Goblin : public Enemy {
private:
    void doDisplay(std::ostream& os) const override;

public:
    explicit Goblin(const std::string& name = "Goblin", int posX = 0, int posY = 0);

    std::unique_ptr<Enemy> clone() const override;

    // Goblins deal low but consistent damage (no variance)
    int computeAttackDamage() const override;

    // On death: gives modest XP to killer
    void onDeath(Character& killer) override;

    // Chases player on sight within 6 tiles; flat attack when adjacent
    void updateAI(Character& hero, const Map& dungeon) override;

    //apply drops
    void applyDrops(EnemyManager& manager) override;
};

#endif // GOBLIN_H
