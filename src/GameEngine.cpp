#include "GameEngine.h"
#include "MathUtils.h"
#include "Weapon.h"

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
    hero.addObserver(&messageLog);
    enemyManager.addObserver(&messageLog);

    (void)hudFont.openFromFile("assets/fonts/PixelPurl.ttf");
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

    loadLevel(0);
}

void GameEngine::loadLevel(int index) {
    currentLevel = index;

    const LevelConfig config = Levelmanager::configFor(index);
    dungeon.generate(config.getNumRooms(), config.getObstacleCount());

    auto [spawnX, spawnY] = dungeon.getRandomFloorTile();
    hero.setPosition(spawnX, spawnY);

    dungeon.placeExit(spawnX, spawnY);

    enemyManager.spawnFromConfig(dungeon, spawnX, spawnY, config);

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

            if (!gameOver && !won) {
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

                    // The exit stays sealed until every enemy on the floor is
                    // defeated the player cant step on it before then.
                    const bool targetIsExit =
                        dungeon.getTile(newX, newY) == TileType::EXIT;
                    const bool exitSealed =
                        targetIsExit && !enemyManager.allDefeated();

                    if (exitSealed) {
                        exitHintActive = true;
                        exitHintClock.restart();
                    }

                    if (!exitSealed
                        && ((enemyPresent && enemyDied)
                            || (!enemyPresent && dungeon.isWalkable(newX, newY)))) {
                        hero.setPosition(newX, newY);
                        updateCamera();

                        if (targetIsExit) { // floor cleared → descend or win
                            if (Levelmanager::isFinalLevel(currentLevel)) {
                                won = true;
                            } else {
                                loadLevel(currentLevel + 1);
                            }
                            continue; // new state; skip enemy turn
                        }
                    }

                    enemyManager.takeTurns(hero, dungeon);

                    if (!hero.isAlive()) gameOver = true;
                }

                // Heal keys
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

    const bool showExitHint = exitHintActive
        && exitHintClock.getElapsedTime().asSeconds() < EXIT_HINT_SECONDS;

    hudRenderer.draw(window, hero,
                     enemyManager.getPotions("small"),
                     enemyManager.getPotions("medium"),
                     enemyManager.getPotions("large"),
                     gameOver,
                     showExitHint,
                     won,
                     currentLevel + 1,
                     Levelmanager::levelCount(),
                     messageLog);

    window.setView(camera);
    window.display();
}