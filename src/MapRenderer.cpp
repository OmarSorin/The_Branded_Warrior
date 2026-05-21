#include "MapRenderer.h"

MapRenderer::MapRenderer(int tileSize) : tileSize(tileSize) {
    // === TILE TEXTURES ===
    const bool hasFloor    = texFloor.loadFromFile("assets/tiles/Tile_2.png");
    const bool hasDoor     = texDoor.loadFromFile("assets/tiles/Tile_1.png");
    const bool hasObstacle = texObstacle.loadFromFile("assets/tiles/obstacle_3.png");

    auto scaleToTile = [&](sf::Sprite& spr, const sf::Texture& tex) {
        auto sz = tex.getSize();
        spr.setScale({static_cast<float>(tileSize) / static_cast<float>(sz.x),
                      static_cast<float>(tileSize) / static_cast<float>(sz.y)});
    };

    if (hasFloor)    { sprFloor.emplace(texFloor);       scaleToTile(*sprFloor,    texFloor); }
    if (hasDoor)     { sprDoor.emplace(texDoor);         scaleToTile(*sprDoor,     texDoor); }
    if (hasObstacle) { sprObstacle.emplace(texObstacle); scaleToTile(*sprObstacle, texObstacle); }

    // Fallback colored rectangle
    tileShape.setSize(sf::Vector2f(static_cast<float>(tileSize), static_cast<float>(tileSize)));
}

void MapRenderer::draw(sf::RenderWindow& window, const Map& dungeon) {
    for (int row = 0; row < dungeon.getHeight(); ++row) {
        for (int col = 0; col < dungeon.getWidth(); ++col) {
            const sf::Vector2f pos = {static_cast<float>(col * tileSize),
                                      static_cast<float>(row * tileSize)};
            const TileType tile    = dungeon.getTile(col, row);

            std::optional<sf::Sprite>* sprOpt = nullptr;
            switch (tile) {
                case TileType::FLOOR:    sprOpt = &sprFloor;    break;
                case TileType::DOOR:     sprOpt = &sprDoor;     break;
                case TileType::OBSTACLE: sprOpt = &sprObstacle; break;
                default: break;
            }

            if (sprOpt && sprOpt->has_value()) {
                (*sprOpt)->setPosition(pos);
                window.draw(**sprOpt);
            } else {
                tileShape.setPosition(pos);
                tileShape.setFillColor(dungeon.getTileColor(col, row));
                window.draw(tileShape);
            }
        }
    }
}
