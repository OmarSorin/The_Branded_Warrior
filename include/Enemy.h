#ifndef ENEMY_H
#define ENEMY_H

#include <memory>
#include <ostream>
#include <string>

class Character; // forward declaration
class Map;
class EnemyManager;

class Enemy {
private:
    std::string name;
    int hp;
    int maxHp;
    int level;
    int posX;
    int posY;

    static int totalEnemiesCreated; // cppcheck-suppress unusedFunction

protected:
    int  detectionRange = 6; // how many tiles away enemy can detect the hero
    bool aggro          = false; // true once enemy has spotted the hero

    // Available to derived classes for healing (e.g. Troll regen)
    virtual void healHp(int amount);

    // NVI: derived classes implement this
    virtual void doDisplay(std::ostream& os) const = 0;

public:
    Enemy(const std::string& name, int hp, int level, int posX = 0, int posY = 0);
    virtual ~Enemy() = default;

    // Virtual constructor (clone) — required for polymorphic copying
    virtual std::unique_ptr<Enemy> clone() const = 0;

    // Thematic pure virtual: each enemy type computes damage differently
    virtual int computeAttackDamage() const = 0;

    // Thematic pure virtual: what happens when this enemy dies
    virtual void onDeath(Character& killer) = 0;

    // Thematic pure virtual: enemy AI — called once per player turn
    // Each enemy type moves and attacks differently
    virtual void updateAI(Character& hero, const Map& dungeon) = 0;

    // NVI: public, non-virtual — calls doDisplay()
    void display(std::ostream& os) const;

    // Accessors
    const std::string& getName() const { return name; }
    int getHp()    const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getLevel() const { return level; }
    int getX()     const { return posX; }
    int getY()     const { return posY; }
    bool isAlive() const { return hp > 0; }

    void setPosition(int x, int y);

    // Returns true if the enemy died
    bool takeDamage(int amount);
    // cppcheck-suppress unusedFunction
    static int getTotalEnemiesCreated() { return totalEnemiesCreated; } // da l incolo ca nu pot sa l fac sa functioneze

    virtual void applyDrops(EnemyManager& manager) = 0;

    friend std::ostream& operator<<(std::ostream& os, const Enemy& e);
};

std::ostream& operator<<(std::ostream& os, const Enemy& e);

#endif // ENEMY_H
