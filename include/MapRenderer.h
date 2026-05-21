#ifndef MAPRENDERER_H
#define MAPRENDERER_H

#include <SFML/Graphics.hpp>
#include <optional>
#include "Map.h"

class MapRenderer {
private:
    int tileSize;

    // Textures
    sf::Texture texFloor;
    sf::Texture texDoor;
    sf::Texture texObstacle;

    // Pre-scaled Sprites
    std::optional<sf::Sprite> sprFloor;
    std::optional<sf::Sprite> sprDoor;
    std::optional<sf::Sprite> sprObstacle;

    // Fallback shape if textures fail
    sf::RectangleShape tileShape;

public:
    explicit MapRenderer(int tileSize);

    void draw(sf::RenderWindow& window, const Map& dungeon);
};

#endif // MAPRENDERER_H
