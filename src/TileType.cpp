#include "TileType.h"

std::ostream &operator<<(std::ostream &os, const TileType &tile) {
  switch (tile) {
  case TileType::WALL:
    os << '#';
    break;
  case TileType::FLOOR:
    os << '.';
    break;
  case TileType::DOOR:
    os << '+';
    break;
  case TileType::OBSTACLE:
    os << 'o';
    break;
  }
  return os;
}
