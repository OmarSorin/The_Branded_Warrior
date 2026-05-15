#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <utility>
#include <vector>

class Map;

// BFS shortest path on the dungeon grid.
// Returns the full path as a list of (x, y) positions including start and goal.
// Returns an empty vector if no path exists.
// The goal tile is included even if it is occupied by the hero (not walkable for others).
std::vector<std::pair<int, int>> findPath(const Map& map,
                                          int startX, int startY,
                                          int goalX,  int goalY);

#endif // PATHFINDING_H
