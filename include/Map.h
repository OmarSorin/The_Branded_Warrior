#ifndef MAP_H
#define MAP_H

#include "Room.h"
#include "TileType.h"
#include <ostream>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

class Map {
private:
  int width;
  int height;
  std::vector<std::vector<TileType>> tiles;
  std::vector<Room> rooms;

  void fillWithWalls();
  void carveRoom(const Room &room);
  void carveHorizontalCorridor(int x1, int x2, int y);
  void carveVerticalCorridor(int y1, int y2, int x);
  void connectRooms(const Room &a, const Room &b);
  void placeDoors();
  bool isAdjacentToDoor(int x, int y) const;
  void placeObstacles(int count);

public:
  Map(int width = 50, int height = 30);

  void generate(int numRooms = 6, int obstacleCount = 10);

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  TileType getTile(int x, int y) const;

  bool isInBounds(int x, int y) const;

  bool isWalkable(int x, int y) const;

  const std::vector<Room> &getRooms() const { return rooms; }

  std::pair<int, int> getRandomFloorTile() const;

  int countAdjacentWalkable(int x, int y) const;

  bool hasLineOfSight(int x1, int y1, int x2, int y2) const;

  sf::Color getTileColor(int x, int y) const;

  friend std::ostream &operator<<(std::ostream &os, const Map &map);
};

std::ostream &operator<<(std::ostream &os, const Map &map);

#endif // MAP_H
