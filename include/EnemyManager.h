#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Character.h"
#include "Map.h"

class EnemyManager {
private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    sf::RectangleShape enemyShape;
    int tileSize;

public:
    explicit EnemyManager(int tileSize);

    void spawnInitialEnemies(const Map& dungeon, int spawnX, int spawnY);

    // Processes player attack on enemy at (newX, newY). Returns true if an enemy is present.
    // enemyDied is set to true if the attack killed the enemy.
    bool handlePlayerAttack(int newX, int newY, Character& hero, 
                            int& smallPotions, int& medPotions, int& largePotions,
                            bool& enemyDied);

    // Executes AI turns for all enemies and processes collisions
    void takeTurns(Character& hero, const Map& dungeon);

    void draw(sf::RenderWindow& window);
};

#endif // ENEMYMANAGER_H
