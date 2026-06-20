#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <string>
#include <utility>
#include <vector>

#include "Range.h"

//made so each floor gets a random number of mobs
class EnemySpawn {
    std::string type;
    util::Range<int> count;

public:
    EnemySpawn(std::string type, util::Range<int> count)
        : type(std::move(type)), count(count) {}
    // cppcheck-suppress unusedFunction
    const std::string &getType() const { return type; }
    // cppcheck-suppress unusedFunction
    const util::Range<int> &getCount() const { return count; }
};

//everything that makes levels different
class LevelConfig {
    int depth;
    int numRooms;
    int obstacleCount;
    std::vector<EnemySpawn> roster;

public:
    LevelConfig(int depth, int numRooms, int obstacleCount)
        : depth(depth), numRooms(numRooms), obstacleCount(obstacleCount) {}

    void addSpawn(const EnemySpawn &spawn) { roster.push_back(spawn); }
    // cppcheck-suppress unusedFunction
    int getDepth() const { return depth; }
    // cppcheck-suppress unusedFunction
    int getNumRooms() const { return numRooms; }
    // cppcheck-suppress unusedFunction
    int getObstacleCount() const { return obstacleCount; }
    // cppcheck-suppress unusedFunction
    const std::vector<EnemySpawn> &getRoster() const { return roster; }
};

//dungeon grows each lvl
class Levelmanager {
    static constexpr int LEVEL_COUNT = 3; // floors to clear before winning

public:
    // cppcheck-suppress unusedFunction
    static LevelConfig configFor(int depth);

    // total number of floors in a run
    // cppcheck-suppress unusedFunction
    static int levelCount() { return LEVEL_COUNT; }

    // true if the given 0-based depth is the final floor
    // cppcheck-suppress unusedFunction
    static bool isFinalLevel(int depth) { return depth >= LEVEL_COUNT - 1; }
};

#endif // LEVELMANAGER_H