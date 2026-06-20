#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <any>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include <unordered_map>
#include "Character.h"
#include "Map.h"
class LevelConfig;
class EnemyManager {
private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    sf::RectangleShape enemyShape;
    int tileSize;
    std::unordered_map<std::string, std::any> registry;

public:
    explicit EnemyManager(int tileSize);

    void spawnFromConfig(const Map& dungeon, int spawnX, int spawnY,
                     const LevelConfig& config);

    // Processes player attack on enemy at (newX, newY). Returns true if an enemy is present.
    // enemyDied is set to true if the attack killed the enemy.
    // old constructor had int& smallPotions, int& medPotions, int& largePotions,
    bool handlePlayerAttack(int newX, int newY, Character& hero,
                            bool& enemyDied);

    // Executes AI turns for all enemies and processes collisions
    void takeTurns(Character& hero, const Map& dungeon);

    void modifyPotions(const std::string& type, int amount);
    int getPotions(const std::string& type) const;
    bool allDefeated() const;


    void draw(sf::RenderWindow& window);

    EnemyManager(const EnemyManager& other);
    EnemyManager& operator=(EnemyManager other);
};

#endif // ENEMYMANAGER_H
