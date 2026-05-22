#include "EnemyManager.h"
#include "Goblin.h"
#include "Orc.h"
#include "Troll.h"
#include "Hobbit.h"
#include <algorithm>

EnemyManager::EnemyManager(int tileSize) : tileSize(tileSize) {
    enemyShape.setSize(sf::Vector2f(static_cast<float>(tileSize), static_cast<float>(tileSize)));
}

void EnemyManager::spawnInitialEnemies(const Map& dungeon, int spawnX, int spawnY) {
    auto spawnEnemy = [&](std::unique_ptr<Enemy> e) {
        auto [ex, ey] = dungeon.getRandomFloorTile();
        while (ex == spawnX && ey == spawnY) {
            auto [nx, ny] = dungeon.getRandomFloorTile();
            ex = nx; ey = ny;
        }
        e->setPosition(ex, ey);
        enemies.push_back(std::move(e));
    };

    spawnEnemy(std::make_unique<Goblin>("Goblin"));
    spawnEnemy(std::make_unique<Goblin>("Goblin 2"));
    spawnEnemy(std::make_unique<Goblin>("Goblin 3"));
    spawnEnemy(std::make_unique<Orc>("Orc"));
    spawnEnemy(std::make_unique<Orc>("Orc 2"));
    spawnEnemy(std::make_unique<Troll>("Troll"));
    spawnEnemy(std::make_unique<Hobbit>("Hobbit"));
}

bool EnemyManager::handlePlayerAttack(int newX, int newY, Character& hero, 
                                      int& smallPotions, int& medPotions, int& largePotions,
                                      bool& enemyDied) {
    enemyDied = false;
    auto it = std::find_if(enemies.begin(), enemies.end(),
        [&](const std::unique_ptr<Enemy>& e) {
            return e->isAlive() && e->getX() == newX && e->getY() == newY;
        });

    if (it != enemies.end()) {
        int dmg = hero.getEquippedWeapon().attack();
        bool died = (*it)->takeDamage(dmg);
        if (died) {
            enemyDied = true;
            // Detect type BEFORE erasing (for potion drop)
            const bool dropsSmall = dynamic_cast<Goblin*>(it->get()) != nullptr;
            const bool dropsMed   = dynamic_cast<Orc*>  (it->get()) != nullptr;
            const bool dropsLarge = dynamic_cast<Troll*>(it->get()) != nullptr;
            
            (*it)->onDeath(hero);
            if (Hobbit* h = dynamic_cast<Hobbit*>(it->get())) {
                if (h->hasLandedLuckyHit()) {
                    hero.gainXp(25); // bonus XP for surviving and killing a lucky Hobbit
                }
            }
            enemies.erase(it);
            
            if      (dropsSmall) ++smallPotions;
            else if (dropsMed)   ++medPotions;
            else if (dropsLarge) ++largePotions;
            else                   ++medPotions;
        }
        return true; // Attack occurred
    }
    return false; // No enemy at that tile
}

void EnemyManager::takeTurns(Character& hero, const Map& dungeon) {
    for (auto& e : enemies) {
        // Snapshot position before AI runs
        int prevX = e->getX(), prevY = e->getY();
        e->updateAI(hero, dungeon);
        
        // Enemy collision: revert if it moved onto another enemy's tile
        bool collided = false;
        for (const auto& other : enemies) {
            if (other.get() == e.get() || !other->isAlive()) continue;
            if (other->getX() == e->getX() && other->getY() == e->getY()) {
                collided = true;
                break;
            }
        }
        if (collided) e->setPosition(prevX, prevY);
    }
    
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e) { return !e->isAlive(); }),
        enemies.end());
}

void EnemyManager::draw(sf::RenderWindow& window) {
    for (const auto& e : enemies) {
        if (!e->isAlive()) continue;
        sf::Color col;
        if      (dynamic_cast<Troll*>(e.get())) col = sf::Color(140,  80, 200);
        else if (dynamic_cast<Orc*>  (e.get())) col = sf::Color(200,  60,  40);
        else if (dynamic_cast<Goblin*>(e.get())) col = sf::Color( 40, 180,  40);
        else                                     col = sf::Color(220,  220,  180);
        
        enemyShape.setFillColor(col);
        enemyShape.setPosition({static_cast<float>(e->getX() * tileSize),
                                 static_cast<float>(e->getY() * tileSize)});
        window.draw(enemyShape);
    }
}

EnemyManager::EnemyManager(const EnemyManager& other)
    : enemyShape(other.enemyShape), tileSize(other.tileSize) {
    for (const auto& e : other.enemies)
        enemies.push_back(e->clone());
}

EnemyManager& EnemyManager::operator=(EnemyManager other) {
    std::swap(enemies, other.enemies);
    std::swap(tileSize, other.tileSize);
    std::swap(enemyShape, other.enemyShape);
    return *this;
}

