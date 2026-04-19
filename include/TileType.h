#ifndef TILETYPE_H
#define TILETYPE_H

#include <ostream>

enum class TileType { WALL, FLOOR, DOOR, OBSTACLE };

std::ostream &operator<<(std::ostream &os, const TileType &tile);

#endif // TILETYPE_H
