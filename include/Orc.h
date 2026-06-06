#ifndef ORC_H
#define ORC_H

#include "Enemy.h"
#include "EnemyManager.h"

class EnemyManager; //forward declaration


// Orc: berserk fighter — damage doubles when HP falls below 30%
class Orc : public Enemy {
private:
    void doDisplay(std::ostream& os) const override;

public:
    explicit Orc(const std::string& name = "Orc", int posX = 0, int posY = 0);

    std::unique_ptr<Enemy> clone() const override;

    // Rage mechanic: doubles damage when below 30% HP
    int computeAttackDamage() const override;

    // On death: gives more XP than a Goblin
    void onDeath(Character& killer) override;

    // Normal: BFS chase on sight (5 tile range)
    // Enraged (<30% HP): greedy charge, detection range doubles to 9 tiles
    void updateAI(Character& hero, const Map& dungeon) override;

    //apply drops
    void applyDrops(EnemyManager& manager) override;

    const sf::Texture* getTexture() const override;
};

#endif // ORC_H
