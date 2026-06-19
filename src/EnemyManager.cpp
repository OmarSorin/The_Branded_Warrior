#include "EnemyManager.h"
#include "EnemyFactory.h"
#include "Goblin.h"
#include "Orc.h"
#include "Troll.h"
#include "Hobbit.h"
#include <algorithm>
#include <iostream>

EnemyManager::EnemyManager(int tileSize) : tileSize(tileSize) {
    enemyShape.setSize(sf::Vector2f(static_cast<float>(tileSize), static_cast<float>(tileSize)));
    registry["small"]  = 0;
    registry["medium"] = 0;
    registry["large"]  = 0;
}

void EnemyManager::spawnInitialEnemies(const Map& dungeon, int spawnX, int spawnY) {
    enemies.clear(); // ensure a clean slate when (re)loading a level

    auto spawnEnemy = [&](std::unique_ptr<Enemy> e) {
        auto [ex, ey] = dungeon.getRandomFloorTile();
        while (ex == spawnX && ey == spawnY) {
            auto [nx, ny] = dungeon.getRandomFloorTile();
            ex = nx; ey = ny;
        }
        e->setPosition(ex, ey);
        enemies.push_back(std::move(e));

    };

    const std::pair<std::string, std::string> roster[] = {
        {"goblin", "Goblin"},
        {"goblin", "Goblin 2"},
        {"goblin", "Goblin 3"},
        {"orc",    "Orc"},
        {"orc",    "Orc 2"},
        {"troll",  "Troll"},
        {"hobbit", "Hobbit"},
    };
    for (const auto& [type, name] : roster) {
        spawnEnemy(EnemyFactory::create(type, name));
    }

    std::cerr << "Total enemies created: " << Enemy::getTotalEnemiesCreated() << "\n";
}

// changed handlePlayerAttack
// old constructor had int& smallPotions, int& medPotions, int& largePotions,
bool EnemyManager::handlePlayerAttack(int newX, int newY, Character& hero,
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
            (*it)->applyDrops(*this);
            (*it)->onDeath(hero);
            enemies.erase(it);
            // here lay an old piece of logic RIP :c
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

        if ( Troll* t= dynamic_cast<Troll*>(e.get())) {
            t->regenerate();
        }

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

        const sf::Texture* tex = e->getTexture();
        if (tex && tex->getSize().x > 0) {
            sf::Sprite sprite(*tex);
            float scaleX = static_cast<float>(tileSize) / tex->getSize().x;
            float scaleY = static_cast<float>(tileSize) / tex->getSize().y;
            sprite.setScale({scaleX, scaleY});
            sprite.setPosition({static_cast<float>(e->getX() * tileSize),
                                static_cast<float>(e->getY() * tileSize)});
            window.draw(sprite);
        } else {
            enemyShape.setTexture(nullptr);
            enemyShape.setFillColor(sf::Color(200, 60, 40));
            enemyShape.setPosition({static_cast<float>(e->getX() * tileSize),
                                    static_cast<float>(e->getY() * tileSize)});
            window.draw(enemyShape);
        }
    }
}

void EnemyManager::modifyPotions(const std::string& type, int amount) {
    auto it = registry.find(type);
    if (it != registry.end())
        std::any_cast<int&>(it->second) += amount;
}

int EnemyManager::getPotions(const std::string& type) const {
    auto it = registry.find(type);
    if (it != registry.end())
        return std::any_cast<int>(it->second);
    return 0;
}

bool EnemyManager::allDefeated() const {
    return std::none_of(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e) { return e->isAlive(); });
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