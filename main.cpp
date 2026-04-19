#include <SFML/Graphics.hpp>

#include "Character.h"
#include "GameDemo.h"
#include "Map.h"
#include "Weapon.h"

int main() {
  // GameDemo demo;
  // demo.run();

  // === MAP SETUP ===
  const int tileSize = 16; // pixels per tile
  Map dungeon(50, 37);     // 50x37 tiles = 800x592 pixels
  dungeon.generate(8, 12);

  // === SFML WINDOW ===
  const int viewWidthTiles = 20;  // how many tiles the camera shows horizontally
  const int viewHeightTiles = 15; // how many tiles the camera shows vertically
  sf::RenderWindow window(
      sf::VideoMode({static_cast<unsigned>(viewWidthTiles * tileSize),
                     static_cast<unsigned>(viewHeightTiles * tileSize)}),
      "Roguelike RPG");
  window.setFramerateLimit(60);

  // === HERO SETUP ===
  Character hero("Aldric", 120, 1, Weapon("Iron Sword", 15, 10));
  auto [spawnX, spawnY] = dungeon.getRandomFloorTile();
  hero.setPosition(spawnX, spawnY);

  // === SPRITES ===
  sf::Texture texRight("assets/guts-right.png");
  sf::Texture texLeft("assets/guts-left.png");
  sf::Texture texUp("assets/guts-up.png");
  sf::Texture texDown("assets/guts-removebg-preview.png");
  sf::Sprite playerSprite(texDown);

  // Scale sprite to fit one tile
  auto texSize = texDown.getSize();
  float scaleX = static_cast<float>(tileSize) / static_cast<float>(texSize.x);
  float scaleY = static_cast<float>(tileSize) / static_cast<float>(texSize.y);
  playerSprite.setScale({scaleX, scaleY});
  playerSprite.setPosition(
      {static_cast<float>(hero.getX() * tileSize),
       static_cast<float>(hero.getY() * tileSize)});

  // === TILE SHAPE (reused for drawing) ===
  sf::RectangleShape tileShape(sf::Vector2f(
      static_cast<float>(tileSize), static_cast<float>(tileSize)));

  // === CAMERA ===
  sf::View camera(sf::FloatRect(
      {0.f, 0.f},
      {static_cast<float>(viewWidthTiles * tileSize),
       static_cast<float>(viewHeightTiles * tileSize)}));

  // Helper to center camera on player and clamp to map edges
  float halfW = static_cast<float>(viewWidthTiles * tileSize) / 2.f;
  float halfH = static_cast<float>(viewHeightTiles * tileSize) / 2.f;
  float mapW = static_cast<float>(dungeon.getWidth() * tileSize);
  float mapH = static_cast<float>(dungeon.getHeight() * tileSize);

  auto updateCamera = [&]() {
    float cx = static_cast<float>(hero.getX() * tileSize) + tileSize / 2.f;
    float cy = static_cast<float>(hero.getY() * tileSize) + tileSize / 2.f;

    // Clamp so camera doesn't go past map edges
    if (cx < halfW) cx = halfW;
    if (cy < halfH) cy = halfH;
    if (cx > mapW - halfW) cx = mapW - halfW;
    if (cy > mapH - halfH) cy = mapH - halfH;

    camera.setCenter({cx, cy});
    window.setView(camera);
  };

  updateCamera();

  // === GAME LOOP ===
  while (window.isOpen()) {
    // 1. POLL EVENTS — one key press = one tile move
    while (auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }

      if (auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        int dx = 0, dy = 0;

        if (keyPressed->code == sf::Keyboard::Key::W) {
          dy = -1;
          playerSprite.setTexture(texUp);
        } else if (keyPressed->code == sf::Keyboard::Key::S) {
          dy = 1;
          playerSprite.setTexture(texDown);
        } else if (keyPressed->code == sf::Keyboard::Key::A) {
          dx = -1;
          playerSprite.setTexture(texLeft);
        } else if (keyPressed->code == sf::Keyboard::Key::D) {
          dx = 1;
          playerSprite.setTexture(texRight);
        }

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

    // 2. DRAW
    window.clear(sf::Color::Black);

    // Draw the map tiles
    for (int row = 0; row < dungeon.getHeight(); ++row) {
      for (int col = 0; col < dungeon.getWidth(); ++col) {
        tileShape.setPosition(
            {static_cast<float>(col * tileSize),
             static_cast<float>(row * tileSize)});

        tileShape.setFillColor(dungeon.getTileColor(col, row));

        window.draw(tileShape);
      }
    }

    // Draw the player on top
    window.draw(playerSprite);
    window.display();
  }

  return 0;
}
