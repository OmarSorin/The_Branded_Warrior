#ifndef TROLL_H
#define TROLL_H

#include "Enemy.h"

// Troll: tanky brute — high HP, heavy hits, regenerates health each turn
class Troll : public Enemy {
private:
    static constexpr int REGEN_AMOUNT   = 5;
    static constexpr int TURN_COOLDOWN  = 2; // moves every 2 player turns
    int turnCounter = 0;

    void doDisplay(std::ostream& os) const override;

public:
    explicit Troll(const std::string& name = "Troll", int posX = 0, int posY = 0);

    std::unique_ptr<Enemy> clone() const override;

    // Trolls hit hard
    int computeAttackDamage() const override;

    // On death: gives the most XP of the basic enemies
    void onDeath(Character& killer) override;

    // Troll-specific: regenerate HP each turn (used with dynamic_cast in EnemyGroup)
    void regenerate();

    // Slow (moves every 2 turns), detects hero without LOS (8 tile range), regenerates on its turn
    void updateAI(Character& hero, const Map& dungeon) override;
};

#endif // TROLL_H
