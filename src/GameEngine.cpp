#include "GameEngine.h"
#include "Weapon.h"
#include "MathUtils.h"

GameEngine::GameEngine()
    : window(sf::VideoMode({1280u, 720u}), "The Branded Warrior"),
      camera(),
      hudFont(),
      dungeon(50, 37),
      hero("Aldric", 120, 1, Weapon("Iron Sword", 15, 999)),
        // hero("Aldric", 120, 1, Weapon("Iron Sword", 1000, 10)), // godmode
      gameOver(false),
      mapRenderer(TILE_SIZE),
      enemyManager(TILE_SIZE),
      hudRenderer(hudFont),
      texRight(), texLeft(), texUp(), texDown(),
      playerSprite(texDown),
      potSmall("Small Potion", 25),
      potMed("Medium Potion", 50),
      potLarge("Large Potion", 100) {

    window.setFramerateLimit(60);

    camera.setSize({
        static_cast<float>(VIEW_WIDTH_TILES * TILE_SIZE),
        static_cast<float>(VIEW_HEIGHT_TILES * TILE_SIZE)
    });
}

void GameEngine::initialize() {
    dungeon.generate(8, 12);

    auto [spawnX, spawnY] = dungeon.getRandomFloorTile();
    hero.setPosition(spawnX, spawnY);

    enemyManager.spawnInitialEnemies(dungeon, spawnX, spawnY);

    (void)hudFont.openFromFile("C:/Windows/Fonts/arial.ttf");
    hudRenderer = HudRenderer(hudFont);

    (void)texRight.loadFromFile("assets/guts-right.png");
    (void)texLeft.loadFromFile("assets/guts-left.png");
    (void)texUp.loadFromFile("assets/guts-up.png");
    (void)texDown.loadFromFile("assets/guts-removebg-preview.png");
    playerSprite.setTexture(texDown, true);

    auto texSize = texDown.getSize();
    float scaleX = static_cast<float>(TILE_SIZE) / static_cast<float>(texSize.x);
    float scaleY = static_cast<float>(TILE_SIZE) / static_cast<float>(texSize.y);
    playerSprite.setScale({scaleX, scaleY});

    updateCamera();
}

void GameEngine::run() {
    while (window.isOpen()) {
        handleInput();
        render();
    }
}

void GameEngine::handleInput() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                window.close();
            }

            if (!gameOver) {
                int dx = 0, dy = 0;
                if      (keyPressed->code == sf::Keyboard::Key::W) { dy = -1; playerSprite.setTexture(texUp, true); }
                else if (keyPressed->code == sf::Keyboard::Key::S) { dy =  1; playerSprite.setTexture(texDown, true); }
                else if (keyPressed->code == sf::Keyboard::Key::A) { dx = -1; playerSprite.setTexture(texLeft, true); }
                else if (keyPressed->code == sf::Keyboard::Key::D) { dx =  1; playerSprite.setTexture(texRight, true); }

                if (dx != 0 || dy != 0) {
                    int newX = hero.getX() + dx;
                    int newY = hero.getY() + dy;

                    bool enemyDied = false;
                    bool enemyPresent = enemyManager.handlePlayerAttack(newX, newY, hero, enemyDied);

                    if ((enemyPresent && enemyDied) || (!enemyPresent && dungeon.isWalkable(newX, newY))) {
                        hero.setPosition(newX, newY);
                        updateCamera();
                    }

                    enemyManager.takeTurns(hero, dungeon);

                    if (!hero.isAlive()) gameOver = true;
                }

                // ── Heal keys ──────────────────────────────────────────────────
                if (keyPressed->code == sf::Keyboard::Key::U && enemyManager.getPotions("small") > 0) {
                    hero.heal(potSmall);
                    enemyManager.modifyPotions("small", -1);
                } else if (keyPressed->code == sf::Keyboard::Key::I && enemyManager.getPotions("medium") > 0) {
                    hero.heal(potMed);
                    enemyManager.modifyPotions("medium", -1);
                } else if (keyPressed->code == sf::Keyboard::Key::O && enemyManager.getPotions("large") > 0) {
                    hero.heal(potLarge);
                    enemyManager.modifyPotions("large", -1);
                }
            }
        }
    }
}

void GameEngine::updateCamera() {
    float halfW = static_cast<float>(VIEW_WIDTH_TILES  * TILE_SIZE) / 2.f;
    float halfH = static_cast<float>(VIEW_HEIGHT_TILES * TILE_SIZE) / 2.f;
    auto mapW  = static_cast<float>(dungeon.getWidth()  * TILE_SIZE);
    auto mapH  = static_cast<float>(dungeon.getHeight() * TILE_SIZE);

    float cx = static_cast<float>(hero.getX() * TILE_SIZE) + TILE_SIZE / 2.f;
    float cy = static_cast<float>(hero.getY() * TILE_SIZE) + TILE_SIZE / 2.f;

    cx = util::clamp(cx, halfW, mapW - halfW);
    cy = util::clamp(cy, halfH, mapH - halfH);

    camera.setCenter({cx, cy});

    playerSprite.setPosition({
        static_cast<float>(hero.getX() * TILE_SIZE),
        static_cast<float>(hero.getY() * TILE_SIZE)
    });
}

void GameEngine::render() {
    window.clear(sf::Color::Black);

    window.setView(camera);

    mapRenderer.draw(window, dungeon);
    enemyManager.draw(window);
    window.draw(playerSprite);

    hudRenderer.draw(window, hero,
                     enemyManager.getPotions("small"),
                     enemyManager.getPotions("medium"),
                     enemyManager.getPotions("large"),
                     gameOver);

    window.setView(camera);
    window.display();
}