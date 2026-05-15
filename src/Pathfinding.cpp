#include "Pathfinding.h"

#include "Map.h"

#include <algorithm>
#include <cstdint>
#include <queue>
#include <unordered_map>

// Pack (x, y) into a single 64-bit key for the visited map
static int64_t encode(int x, int y) {
    return (static_cast<int64_t>(x) << 32) | static_cast<uint32_t>(y);
}

std::vector<std::pair<int, int>> findPath(const Map& map,
                                          int startX, int startY,
                                          int goalX,  int goalY) {
    if (startX == goalX && startY == goalY) return {{startX, startY}};

    // cameFrom maps each visited tile to the tile we reached it from
    std::unordered_map<int64_t, std::pair<int, int>> cameFrom;
    std::queue<std::pair<int, int>> frontier;

    cameFrom[encode(startX, startY)] = {-1, -1};
    frontier.push({startX, startY});

    const int dx[] = { 0,  0,  1, -1 };
    const int dy[] = { 1, -1,  0,  0 };

    bool found = false;
    while (!frontier.empty() && !found) {
        auto [cx, cy] = frontier.front();
        frontier.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            int64_t key = encode(nx, ny);

            if (cameFrom.count(key)) continue;

            // Allow the goal tile even if the hero is standing there
            bool isGoal = (nx == goalX && ny == goalY);
            if (!isGoal && !map.isWalkable(nx, ny)) continue;

            cameFrom[key] = {cx, cy};

            if (isGoal) {
                found = true;
                break;
            }
            frontier.push({nx, ny});
        }
    }

    if (!found) return {};

    // Reconstruct path from goal back to start, then reverse
    std::vector<std::pair<int, int>> path;
    std::pair<int, int> cur = {goalX, goalY};
    while (cur.first != -1) {
        path.push_back(cur);
        cur = cameFrom[encode(cur.first, cur.second)];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
