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

    const std::string &getType() const { return type; }

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

    int getDepth() const { return depth; }

    int getNumRooms() const { return numRooms; }

    int getObstacleCount() const { return obstacleCount; }

    const std::vector<EnemySpawn> &getRoster() const { return roster; }
};

//dungeon grows each lvl
class Levelmanager {
    static constexpr int LEVEL_COUNT = 3; // floors to clear before winning

public:
    LevelConfig configFor(int depth) const;

    // total number of floors in a run
    int levelCount() const { return LEVEL_COUNT; }

    // true if the given 0-based depth is the final floor
    bool isFinalLevel(int depth) const { return depth >= LEVEL_COUNT - 1; }
};

#endif // LEVELMANAGER_H