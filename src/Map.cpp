#include "Map.h"

#include <algorithm>
#include <cstdlib>
#include <random>

void Map::fillWithWalls() {
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      tiles[row][col] = TileType::WALL;
    }
  }
}

void Map::carveRoom(const Room &room) {
  for (int row = room.getY(); row < room.getY() + room.getHeight(); ++row) {
    for (int col = room.getX(); col < room.getX() + room.getWidth(); ++col) {
      if (isInBounds(col, row)) {
        tiles[row][col] = TileType::FLOOR;
      }
    }
  }
}

void Map::carveHorizontalCorridor(int x1, int x2, int y) {
  int startX = std::min(x1, x2);
  int endX = std::max(x1, x2);
  for (int col = startX; col <= endX; ++col) {
    if (isInBounds(col, y)) {
      tiles[y][col] = TileType::FLOOR;
    }
  }
}

void Map::carveVerticalCorridor(int y1, int y2, int x) {
  int startY = std::min(y1, y2);
  int endY = std::max(y1, y2);
  for (int row = startY; row <= endY; ++row) {
    if (isInBounds(x, row)) {
      tiles[row][x] = TileType::FLOOR;
    }
  }
}

void Map::connectRooms(const Room &a, const Room &b) {
  static std::mt19937 rng{std::random_device{}()};
  std::uniform_int_distribution<int> coinFlip(0, 1);

  int ax = a.centerX();
  int ay = a.centerY();
  int bx = b.centerX();
  int by = b.centerY();

  if (coinFlip(rng) == 0) {
    carveHorizontalCorridor(ax, bx, ay);
    carveVerticalCorridor(ay, by, bx);
  } else {
    carveVerticalCorridor(ay, by, ax);
    carveHorizontalCorridor(ax, bx, by);
  }
}

void Map::placeDoors() {
  for (int row = 1; row < height - 1; ++row) {
    for (int col = 1; col < width - 1; ++col) {
      if (tiles[row][col] != TileType::FLOOR)
        continue;

      bool wallNorth = tiles[row - 1][col] == TileType::WALL;
      bool wallSouth = tiles[row + 1][col] == TileType::WALL;
      bool wallWest = tiles[row][col - 1] == TileType::WALL;
      bool wallEast = tiles[row][col + 1] == TileType::WALL;

      bool floorNorth = tiles[row - 1][col] == TileType::FLOOR ||
                        tiles[row - 1][col] == TileType::DOOR;
      bool floorSouth = tiles[row + 1][col] == TileType::FLOOR ||
                        tiles[row + 1][col] == TileType::DOOR;
      bool floorWest = tiles[row][col - 1] == TileType::FLOOR ||
                       tiles[row][col - 1] == TileType::DOOR;
      bool floorEast = tiles[row][col + 1] == TileType::FLOOR ||
                       tiles[row][col + 1] == TileType::DOOR;

      bool verticalPassage = wallWest && wallEast && floorNorth && floorSouth;
      bool horizontalPassage =
          wallNorth && wallSouth && floorWest && floorEast;

      if (verticalPassage || horizontalPassage) {
        tiles[row][col] = TileType::DOOR;
      }
    }
  }
}

bool Map::isAdjacentToDoor(int x, int y) const {
  return (isInBounds(x, y - 1) && tiles[y - 1][x] == TileType::DOOR) ||
         (isInBounds(x, y + 1) && tiles[y + 1][x] == TileType::DOOR) ||
         (isInBounds(x - 1, y) && tiles[y][x - 1] == TileType::DOOR) ||
         (isInBounds(x + 1, y) && tiles[y][x + 1] == TileType::DOOR);
}

void Map::placeObstacles(int count) {
  static std::mt19937 rng{std::random_device{}()};
  std::uniform_int_distribution<int> distX(1, width - 2);
  std::uniform_int_distribution<int> distY(1, height - 2);

  int placed = 0;
  int attempts = 0;
  while (placed < count && attempts < count * 10) {
    int x = distX(rng);
    int y = distY(rng);
    ++attempts;

    if (tiles[y][x] == TileType::FLOOR &&
        countAdjacentWalkable(x, y) >= 6 &&
        !isAdjacentToDoor(x, y)) {
      tiles[y][x] = TileType::OBSTACLE;
      ++placed;
    }
  }
}

Map::Map(int width, int height)
    : width(width), height(height),
      tiles(height, std::vector<TileType>(width, TileType::WALL)) {}

void Map::generate(int numRooms, int obstacleCount) {
  static std::mt19937 rng{std::random_device{}()};

  fillWithWalls();
  rooms.clear();

  int minRoomSize = 4;
  int maxRoomSize = 9;

  std::uniform_int_distribution<int> roomW(minRoomSize, maxRoomSize);
  std::uniform_int_distribution<int> roomH(minRoomSize, maxRoomSize);

  int maxAttempts = numRooms * 20;
  for (int attempt = 0;
       attempt < maxAttempts && static_cast<int>(rooms.size()) < numRooms;
       ++attempt) {
    int w = roomW(rng);
    int h = roomH(rng);

    std::uniform_int_distribution<int> posX(1, width - w - 1);
    std::uniform_int_distribution<int> posY(1, height - h - 1);

    Room newRoom(posX(rng), posY(rng), w, h);

    bool overlaps = false;
    for (const auto &existing : rooms) {
      if (newRoom.intersects(existing, 2)) {
        overlaps = true;
        break;
      }
    }

    if (!overlaps) {
      carveRoom(newRoom);
      if (!rooms.empty()) {
        connectRooms(rooms.back(), newRoom);
      }
      rooms.push_back(newRoom);
    }
  }

  placeDoors();
  placeObstacles(obstacleCount);
}

TileType Map::getTile(int x, int y) const {
  if (!isInBounds(x, y))
    return TileType::WALL;
  return tiles[y][x];
}

bool Map::isInBounds(int x, int y) const {
  return x >= 0 && x < width && y >= 0 && y < height;
}

bool Map::isWalkable(int x, int y) const {
  if (!isInBounds(x, y))
    return false;
  TileType t = tiles[y][x];
  return t == TileType::FLOOR || t == TileType::DOOR;
}

std::pair<int, int> Map::getRandomFloorTile() const {
  static std::mt19937 rng{std::random_device{}()};

  std::vector<std::pair<int, int>> floorTiles;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      if (tiles[row][col] == TileType::FLOOR) {
        floorTiles.emplace_back(col, row);
      }
    }
  }

  if (floorTiles.empty()) {
    return {1, 1};
  }

  std::uniform_int_distribution<int> dist(
      0, static_cast<int>(floorTiles.size()) - 1);
  return floorTiles[dist(rng)];
}

int Map::countAdjacentWalkable(int x, int y) const {
  int count = 0;
  for (int dy = -1; dy <= 1; ++dy) {
    for (int dx = -1; dx <= 1; ++dx) {
      if (dx == 0 && dy == 0)
        continue;
      if (isWalkable(x + dx, y + dy)) {
        ++count;
      }
    }
  }
  return count;
}

bool Map::hasLineOfSight(int x1, int y1, int x2, int y2) const {
  int dx = std::abs(x2 - x1);
  int dy = std::abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  int cx = x1;
  int cy = y1;

  while (cx != x2 || cy != y2) {
    if ((cx != x1 || cy != y1) && !isWalkable(cx, cy)) {
      return false;
    }

    int err2 = 2 * err;
    if (err2 > -dy) {
      err -= dy;
      cx += sx;
    }
    if (err2 < dx) {
      err += dx;
      cy += sy;
    }
  }

  return true;
}

sf::Color Map::getTileColor(int x, int y) const {
  switch (getTile(x, y)) {
  case TileType::WALL:     return sf::Color(40, 40, 50);
  case TileType::FLOOR:    return sf::Color(120, 110, 100);
  case TileType::DOOR:     return sf::Color(160, 120, 60);
  case TileType::OBSTACLE: return sf::Color(80, 80, 90);
  default:                 return sf::Color::Black;
  }
}

std::ostream &operator<<(std::ostream &os, const Map &map) {
  for (int row = 0; row < map.getHeight(); ++row) {
    for (int col = 0; col < map.getWidth(); ++col) {
      os << map.getTile(col, row);
    }
    os << '\n';
  }
  os << "Rooms: " << map.getRooms().size();
  for (size_t i = 0; i < map.getRooms().size(); ++i) {
    os << "\n  " << (i + 1) << ". " << map.getRooms()[i];
  }
  return os;
}
