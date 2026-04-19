#ifndef ROOM_H
#define ROOM_H

#include <ostream>

class Room {
private:
  int x, y, width, height;

public:
  Room(int x, int y, int w, int h);

  int getX() const { return x; }
  int getY() const { return y; }
  int getWidth() const { return width; }
  int getHeight() const { return height; }

  int centerX() const { return x + width / 2; }
  int centerY() const { return y + height / 2; }

  bool intersects(const Room &other, int padding = 1) const;
};

std::ostream &operator<<(std::ostream &os, const Room &room);

#endif // ROOM_H
