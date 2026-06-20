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
    int numRooms;
    int obstacleCount;
    std::vector<EnemySpawn> roster;

public:
    LevelConfig(int numRooms, int obstacleCount)
        : numRooms(numRooms), obstacleCount(obstacleCount) {}

    void addSpawn(const EnemySpawn &spawn) { roster.push_back(spawn); }

    int getNumRooms() const { return numRooms; }

    int getObstacleCount() const { return obstacleCount; }

    const std::vector<EnemySpawn> &getRoster() const { return roster; }
};

//dungeon grows each lvl
class Levelmanager {
public:
    LevelConfig configFor(int depth) const;
};

#endif // LEVELMANAGER_H