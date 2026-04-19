#include "Room.h"

Room::Room(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}

// cppcheck-suppress unusedFunction
bool Room::intersects(const Room &other, int padding) const {
  return !(x - padding >= other.x + other.width ||
           x + width + padding <= other.x ||
           y - padding >= other.y + other.height ||
           y + height + padding <= other.y);
}

std::ostream &operator<<(std::ostream &os, const Room &room) {
  os << "Room[pos=(" << room.getX() << "," << room.getY()
     << "), size=" << room.getWidth() << "x" << room.getHeight()
     << ", center=(" << room.centerX() << "," << room.centerY() << ")]";
  return os;
}
