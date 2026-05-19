#include <SFML/Graphics.hpp>

#include <algorithm>
#include <memory>
#include <optional>
#include <string>

#include "Potion.h"

#include "Character.h"
#include "GameDemo.h"
#include "Goblin.h"
#include "HudRenderer.h"
#include "Map.h"
#include "Orc.h"
#include "Troll.h"
#include "Weapon.h"

int main() {
  // === MAP SETUP ===
  const int tileSize = 16; // pixels per tile
  Map dungeon(50, 37);     // 50x37 tiles = 800x592 pixels
  dungeon.generate(8, 12);

  // === SFML WINDOW ===
  const int viewWidthTiles = 20;  // tiles shown horizontally
  const int viewHeightTiles = 15; // tiles shown vertically
  // Window opens at 720p; the game view (320x240) is scaled up automatically.
  sf::RenderWindow window(
      sf::VideoMode({1280u, 720u}),
      "The Branded Warrior");
  window.setFramerateLimit(60);

  // === HERO SETUP ===
  Character hero("Aldric", 120, 1, Weapon("Iron Sword", 15, 10));
  auto [spawnX, spawnY] = dungeon.getRandomFloorTile();
  hero.setPosition(spawnX, spawnY);

  // === ENEMY SPAWNING ===
  std::vector<std::unique_ptr<Enemy>> enemies;
  {
    auto spawnEnemy = [&](std::unique_ptr<Enemy> e) {
      auto [ex, ey] = dungeon.getRandomFloorTile();
      while (ex == spawnX && ey == spawnY) {
        auto [nx, ny] = dungeon.getRandomFloorTile();
        ex = nx; ey = ny;
      }
      e->setPosition(ex, ey);
      enemies.push_back(std::move(e));
    };
    spawnEnemy(std::make_unique<Goblin>("Goblin"));
    spawnEnemy(std::make_unique<Goblin>("Goblin 2"));
    spawnEnemy(std::make_unique<Goblin>("Goblin 3"));
    spawnEnemy(std::make_unique<Orc>("Orc"));
    spawnEnemy(std::make_unique<Orc>("Orc 2"));
    spawnEnemy(std::make_unique<Troll>("Troll"));
  }

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
  const bool hasFloor    = texFloor.loadFromFile("assets/tiles/Tile_2.png");
  // const bool hasWall     = texWall.loadFromFile("assets/tiles/Tile_3.png");
  const bool hasDoor     = texDoor.loadFromFile("assets/tiles/Tile_1.png");
  const bool hasObstacle = texObstacle.loadFromFile("assets/tiles/obstacle_3.png");

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

  // === HUD SETUP ===
  sf::Font hudFont("C:/Windows/Fonts/arial.ttf");
  HudRenderer hudRenderer(hudFont);

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

  // Enemy rendering shape (color varies by type)
  sf::RectangleShape enemyShape(sf::Vector2f(
      static_cast<float>(tileSize), static_cast<float>(tileSize)));

  // === POTION SYSTEM ===
  // Each potion type has a count, a Potion object for healing, and a key binding.
  int smallPotions = 0; // Goblin drop  — heals  25 HP  — key U
  int medPotions   = 0; // Orc drop     — heals  50 HP  — key I
  int largePotions = 0; // Troll drop   — heals 100 HP  — key O
  const Potion potSmall("Small Potion",  25);
  const Potion potMed  ("Medium Potion", 50);
  const Potion potLarge("Large Potion", 100);

  // === GAME LOOP ===
  bool gameOver = false;

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

        // ── Movement (WASD) — blocked when dead ──────────────────────────
        if (!gameOver) {
          int dx = 0, dy = 0;
          if      (keyPressed->code == sf::Keyboard::Key::W) { dy = -1; playerSprite.setTexture(texUp); }
          else if (keyPressed->code == sf::Keyboard::Key::S) { dy =  1; playerSprite.setTexture(texDown); }
          else if (keyPressed->code == sf::Keyboard::Key::A) { dx = -1; playerSprite.setTexture(texLeft); }
          else if (keyPressed->code == sf::Keyboard::Key::D) { dx =  1; playerSprite.setTexture(texRight); }

          if (dx != 0 || dy != 0) {
            int newX = hero.getX() + dx;
            int newY = hero.getY() + dy;

            // Bump into enemy = attack it
            auto it = std::find_if(enemies.begin(), enemies.end(),
                [&](const std::unique_ptr<Enemy>& e) {
                  return e->isAlive() && e->getX() == newX && e->getY() == newY;
                });

            if (it != enemies.end()) {
              int dmg = hero.getEquippedWeapon().attack();
              bool died = (*it)->takeDamage(dmg);
              if (died) {
                // Detect type BEFORE erasing (for potion drop)
                const bool dropsSmall = dynamic_cast<Goblin*>(it->get()) != nullptr;
                const bool dropsMed   = dynamic_cast<Orc*>  (it->get()) != nullptr;
                const bool dropsLarge = dynamic_cast<Troll*>(it->get()) != nullptr;
                (*it)->onDeath(hero);
                enemies.erase(it);
                if      (dropsSmall) ++smallPotions;
                else if (dropsMed)   ++medPotions;
                else if (dropsLarge) ++largePotions;
                // Advance on kill: step into the tile the enemy freed
                hero.setPosition(newX, newY);
                playerSprite.setPosition(
                    {static_cast<float>(newX * tileSize),
                     static_cast<float>(newY * tileSize)});
                updateCamera();
              }
            } else if (dungeon.isWalkable(newX, newY)) {
              hero.setPosition(newX, newY);
              playerSprite.setPosition(
                  {static_cast<float>(newX * tileSize),
                   static_cast<float>(newY * tileSize)});
              updateCamera();
            }

            // Enemy turn — each enemy acts once after the player
            for (auto& e : enemies) {
              // Snapshot position before AI runs
              int prevX = e->getX(), prevY = e->getY();
              e->updateAI(hero, dungeon);
              // Enemy collision: revert if it moved onto another enemy's tile
              bool collided = false;
              for (const auto& other : enemies) {
                if (other.get() == e.get() || !other->isAlive()) continue;
                if (other->getX() == e->getX() && other->getY() == e->getY()) {
                  collided = true;
                  break;
                }
              }
              if (collided) e->setPosition(prevX, prevY);
            }
            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                    [](const std::unique_ptr<Enemy>& e) { return !e->isAlive(); }),
                enemies.end());

            // Check hero death after enemies have acted
            if (!hero.isAlive()) gameOver = true;
          }

          // ── Heal keys (free action — no enemy turn) ──────────────────────
          if (keyPressed->code == sf::Keyboard::Key::U && smallPotions > 0) {
            hero.heal(potSmall);
            --smallPotions;
          } else if (keyPressed->code == sf::Keyboard::Key::I && medPotions > 0) {
            hero.heal(potMed);
            --medPotions;
          } else if (keyPressed->code == sf::Keyboard::Key::O && largePotions > 0) {
            hero.heal(potLarge);
            --largePotions;
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

    // Draw enemies
    for (const auto& e : enemies) {
      if (!e->isAlive()) continue;
      sf::Color col;
      if      (dynamic_cast<Troll*>(e.get())) col = sf::Color(140,  80, 200);
      else if (dynamic_cast<Orc*>  (e.get())) col = sf::Color(200,  60,  40);
      else                                     col = sf::Color( 40, 180,  40);
      enemyShape.setFillColor(col);
      enemyShape.setPosition({static_cast<float>(e->getX() * tileSize),
                               static_cast<float>(e->getY() * tileSize)});
      window.draw(enemyShape);
    }

    // Draw player on top
    window.draw(playerSprite);

    // ── Draw HUD ──────────────────────────────────────────────────────────
    hudRenderer.draw(window, hero, smallPotions, medPotions, largePotions, gameOver);

    window.setView(camera);
    window.display();
  }

  return 0;
}
