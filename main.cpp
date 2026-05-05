#include <SFML/Graphics.hpp>

#include <optional>

#include "Character.h"
#include "GameDemo.h"
#include "Map.h"
#include "Weapon.h"

int main() {
  // === MAP SETUP ===
  const int tileSize = 16; // pixels per tile
  Map dungeon(50, 37);     // 50x37 tiles = 800x592 pixels
  dungeon.generate(8, 12);

  // === SFML WINDOW ===
  const int viewWidthTiles = 20;  // tiles shown horizontally
  const int viewHeightTiles = 15; // tiles shown vertically
  sf::RenderWindow window(
      sf::VideoMode({static_cast<unsigned>(viewWidthTiles * tileSize),
                     static_cast<unsigned>(viewHeightTiles * tileSize)}),
      "The Branded Warrior");
  window.setFramerateLimit(60);

  // === HERO SETUP ===
  Character hero("Aldric", 120, 1, Weapon("Iron Sword", 15, 10));
  auto [spawnX, spawnY] = dungeon.getRandomFloorTile();
  hero.setPosition(spawnX, spawnY);

  // === PLAYER SPRITES ===
  sf::Texture texRight("assets/guts-right.png");
  sf::Texture texLeft("assets/guts-left.png");
  sf::Texture texUp("assets/guts-up.png");
  sf::Texture texDown("assets/guts-removebg-preview.png");
  sf::Sprite playerSprite(texDown);

  // Scale player sprite to fit one tile
  auto texSize = texDown.getSize();
  float scaleX = static_cast<float>(tileSize) / static_cast<float>(texSize.x);
  float scaleY = static_cast<float>(tileSize) / static_cast<float>(texSize.y);
  playerSprite.setScale({scaleX, scaleY});
  playerSprite.setPosition(
      {static_cast<float>(hero.getX() * tileSize),
       static_cast<float>(hero.getY() * tileSize)});

  // === TILE TEXTURES ===
  // Place 16x16 pixel PNG files in assets/tiles/.
  // If a file is missing the tile falls back to a flat color automatically.
  sf::Texture texFloor, texWall, texDoor, texObstacle;
  const bool hasFloor    = texFloor.loadFromFile("assets/tiles/Tile_84.png");
  // const bool hasWall     = texWall.loadFromFile("assets/tiles/Tile_03.png");
  const bool hasDoor     = texDoor.loadFromFile("assets/tiles/Tile_45.png");
  const bool hasObstacle = texObstacle.loadFromFile("assets/tiles/6.png");

  // Helper: scale a sprite so it always fills exactly one tile,
  // regardless of the source image resolution.
  auto scaleToTile = [&](sf::Sprite& spr, const sf::Texture& tex) {
    auto sz = tex.getSize();
    spr.setScale({static_cast<float>(tileSize) / static_cast<float>(sz.x),
                  static_cast<float>(tileSize) / static_cast<float>(sz.y)});
  };

  // Build one sprite per tile type (only when the texture loaded successfully).
  // std::optional lets us safely skip types whose PNG is not yet ready.
  std::optional<sf::Sprite> sprFloor, sprWall, sprDoor, sprObstacle;
  if (hasFloor)    { sprFloor.emplace(texFloor);       scaleToTile(*sprFloor,    texFloor); }
  // if (hasWall)     { sprWall.emplace(texWall);         scaleToTile(*sprWall,     texWall); }
  if (hasDoor)     { sprDoor.emplace(texDoor);         scaleToTile(*sprDoor,     texDoor); }
  if (hasObstacle) { sprObstacle.emplace(texObstacle); scaleToTile(*sprObstacle, texObstacle); }

  // Fallback colored rectangle (used when a texture is not yet available)
  sf::RectangleShape tileShape(sf::Vector2f(
      static_cast<float>(tileSize), static_cast<float>(tileSize)));

  // === CAMERA ===
  sf::View camera(sf::FloatRect(
      {0.f, 0.f},
      {static_cast<float>(viewWidthTiles * tileSize),
       static_cast<float>(viewHeightTiles * tileSize)}));

  float halfW = static_cast<float>(viewWidthTiles  * tileSize) / 2.f;
  float halfH = static_cast<float>(viewHeightTiles * tileSize) / 2.f;
  float mapW  = static_cast<float>(dungeon.getWidth()  * tileSize);
  float mapH  = static_cast<float>(dungeon.getHeight() * tileSize);

  auto updateCamera = [&]() {
    float cx = static_cast<float>(hero.getX() * tileSize) + tileSize / 2.f;
    float cy = static_cast<float>(hero.getY() * tileSize) + tileSize / 2.f;
    if (cx < halfW)      cx = halfW;
    if (cy < halfH)      cy = halfH;
    if (cx > mapW - halfW) cx = mapW - halfW;
    if (cy > mapH - halfH) cy = mapH - halfH;
    camera.setCenter({cx, cy});
    window.setView(camera);
  };
  updateCamera();

  // === GAME LOOP ===
  while (window.isOpen()) {
    // 1. POLL EVENTS
    while (auto event = window.pollEvent()) {
      // Close window button
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }

      if (auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        // ── Close on Escape or X ──────────────────────────────────────────
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
          window.close();
        }

        // ── Movement (WASD) ───────────────────────────────────────────────
        int dx = 0, dy = 0;
        if      (keyPressed->code == sf::Keyboard::Key::W) { dy = -1; playerSprite.setTexture(texUp); }
        else if (keyPressed->code == sf::Keyboard::Key::S) { dy =  1; playerSprite.setTexture(texDown); }
        else if (keyPressed->code == sf::Keyboard::Key::A) { dx = -1; playerSprite.setTexture(texLeft); }
        else if (keyPressed->code == sf::Keyboard::Key::D) { dx =  1; playerSprite.setTexture(texRight); }

        if (dx != 0 || dy != 0) {
          int newX = hero.getX() + dx;
          int newY = hero.getY() + dy;
          if (dungeon.isWalkable(newX, newY)) {
            hero.setPosition(newX, newY);
            playerSprite.setPosition(
                {static_cast<float>(newX * tileSize),
                 static_cast<float>(newY * tileSize)});
            updateCamera();
          }
        }
      }
    }

    // 2. DRAW
    window.clear(sf::Color::Black);

    for (int row = 0; row < dungeon.getHeight(); ++row) {
      for (int col = 0; col < dungeon.getWidth(); ++col) {
        const sf::Vector2f pos = {static_cast<float>(col * tileSize),
                                  static_cast<float>(row * tileSize)};
        const TileType tile    = dungeon.getTile(col, row);

        // Pick the pre-built sprite for this tile type (if its texture loaded)
        std::optional<sf::Sprite>* sprOpt = nullptr;
        switch (tile) {
          case TileType::FLOOR:    sprOpt = &sprFloor;    break;
          case TileType::WALL:     sprOpt = &sprWall;     break;
          case TileType::DOOR:     sprOpt = &sprDoor;     break;
          case TileType::OBSTACLE: sprOpt = &sprObstacle; break;
          default: break;
        }

        if (sprOpt && sprOpt->has_value()) {
          // Draw textured tile
          (*sprOpt)->setPosition(pos);
          window.draw(**sprOpt);
        } else {
          // Fallback: flat color (original behavior)
          tileShape.setPosition(pos);
          tileShape.setFillColor(dungeon.getTileColor(col, row));
          window.draw(tileShape);
        }
      }
    }

    // Draw player on top
    window.draw(playerSprite);
    window.display();
  }

  return 0;
}
