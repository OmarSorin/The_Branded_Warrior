#include <SFML/Graphics.hpp>

#include <algorithm>
#include <memory>
#include <optional>
#include <string>

#include "Character.h"
#include "GameDemo.h"
#include "Goblin.h"
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

  // === HP HUD ===
  sf::Font hudFont("C:/Windows/Fonts/arial.ttf");

  const float hpBarMaxW = 200.f;
  const float hpBarH    =  20.f;
  const float hudPad    =   6.f;
  const float hudX      =  16.f;
  const float hudY      =  16.f;

  sf::RectangleShape hpBg({hpBarMaxW + hudPad * 2, hpBarH + hudPad * 2});
  hpBg.setFillColor(sf::Color(0, 0, 0, 180));
  hpBg.setOutlineColor(sf::Color(160, 30, 30));
  hpBg.setOutlineThickness(2.f);
  hpBg.setPosition({hudX, hudY});

  sf::RectangleShape hpBar({hpBarMaxW, hpBarH});
  hpBar.setFillColor(sf::Color(200, 30, 30));
  hpBar.setPosition({hudX + hudPad, hudY + hudPad});

  sf::Text hpText(hudFont, "", 20);
  hpText.setFillColor(sf::Color::White);

  // === XP HUD ===
  const float xpBarY = hudY + hpBarH + hudPad * 2 + 4.f; // just below HP bar

  sf::RectangleShape xpBg({hpBarMaxW + hudPad * 2, hpBarH + hudPad * 2});
  xpBg.setFillColor(sf::Color(0, 0, 0, 180));
  xpBg.setOutlineColor(sf::Color(30, 80, 160));
  xpBg.setOutlineThickness(2.f);
  xpBg.setPosition({hudX, xpBarY});

  sf::RectangleShape xpBar({0.f, hpBarH});
  xpBar.setFillColor(sf::Color(50, 120, 220));
  xpBar.setPosition({hudX + hudPad, xpBarY + hudPad});

  sf::Text xpText(hudFont, "", 20);
  xpText.setFillColor(sf::Color::White);

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

          // Bump into enemy = attack it
          auto it = std::find_if(enemies.begin(), enemies.end(),
              [&](const std::unique_ptr<Enemy>& e) {
                return e->isAlive() && e->getX() == newX && e->getY() == newY;
              });

          if (it != enemies.end()) {
            int dmg = hero.getEquippedWeapon().attack();
            bool died = (*it)->takeDamage(dmg);
            if (died) {
              (*it)->onDeath(hero);
              enemies.erase(it);
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
            e->updateAI(hero, dungeon);
          }
          enemies.erase(
              std::remove_if(enemies.begin(), enemies.end(),
                  [](const std::unique_ptr<Enemy>& e) { return !e->isAlive(); }),
              enemies.end());
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

    // ── Draw HP HUD (screen-space) ────────────────────────────────────────
    window.setView(window.getDefaultView());

    float hpRatio = static_cast<float>(hero.getHp())
                  / static_cast<float>(hero.getMaxHp());
    if (hpRatio < 0.f) hpRatio = 0.f;
    hpBar.setSize({hpBarMaxW * hpRatio, hpBarH});

    hpText.setString(std::to_string(hero.getHp()) + "/" + std::to_string(hero.getMaxHp()));
    hpText.setPosition({hudX + hpBarMaxW + hudPad * 2 + 6.f, hudY});

    window.draw(hpBg);
    window.draw(hpBar);
    window.draw(hpText);

    // XP bar
    float xpRatio = static_cast<float>(hero.getXp())
                  / static_cast<float>(hero.getXpForNextLevel());
    if (xpRatio < 0.f) xpRatio = 0.f;
    if (xpRatio > 1.f) xpRatio = 1.f;
    xpBar.setSize({hpBarMaxW * xpRatio, hpBarH});

    xpText.setString("Lv" + std::to_string(hero.getLevel()) + "  "
                     + std::to_string(hero.getXp()) + "/" + std::to_string(hero.getXpForNextLevel()));
    xpText.setPosition({hudX + hpBarMaxW + hudPad * 2 + 6.f, xpBarY});

    window.draw(xpBg);
    window.draw(xpBar);
    window.draw(xpText);

    window.setView(camera);
    window.display();
  }

  return 0;
}
