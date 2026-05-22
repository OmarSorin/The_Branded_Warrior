#include "GameEngine.h"
#include "Weapon.h"

GameEngine::GameEngine()
    : window(sf::VideoMode({1280u, 720u}), "The Branded Warrior"),
      camera(),
      hudFont(),
      dungeon(50, 37),
      hero("Aldric", 120, 1, Weapon("Iron Sword", 15, 999)),
        // hero("Aldric", 120, 1, Weapon("Iron Sword", 1000, 10)), //godmode
      gameOver(false),
      mapRenderer(TILE_SIZE),
      enemyManager(TILE_SIZE),
      hudRenderer(hudFont), // Initialize with empty font first, we load it in initialize()
      texRight(), texLeft(), texUp(), texDown(),
      playerSprite(texDown),
      smallPotions(0), medPotions(0), largePotions(0),
      potSmall("Small Potion", 25),
      potMed("Medium Potion", 50),
      potLarge("Large Potion", 100) {
    
    window.setFramerateLimit(60);
    
    // Setup camera
    camera.setSize({
        static_cast<float>(VIEW_WIDTH_TILES * TILE_SIZE),
        static_cast<float>(VIEW_HEIGHT_TILES * TILE_SIZE)
    });
}

void GameEngine::initialize() {
    // Generate map
    dungeon.generate(8, 12);

    // Spawn Hero
    auto [spawnX, spawnY] = dungeon.getRandomFloorTile();
    hero.setPosition(spawnX, spawnY);

    // Spawn Enemies
    enemyManager.spawnInitialEnemies(dungeon, spawnX, spawnY);

    // Load Fonts & Setup HUD
    (void)hudFont.openFromFile("C:/Windows/Fonts/arial.ttf");
    hudRenderer = HudRenderer(hudFont); // Reconstruct now that font is loaded

    // Load Player Sprites
    (void)texRight.loadFromFile("assets/guts-right.png");
    (void)texLeft.loadFromFile("assets/guts-left.png");
    (void)texUp.loadFromFile("assets/guts-up.png");
    (void)texDown.loadFromFile("assets/guts-removebg-preview.png");
    playerSprite.setTexture(texDown, true);

    // Scale player sprite
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

                    // 1. Try to attack enemy
                    bool enemyDied = false;
                    bool enemyPresent = enemyManager.handlePlayerAttack(newX, newY, hero, smallPotions, medPotions, largePotions, enemyDied);
                    
                    // 2. Advance on kill, or move if walkable
                    if ((enemyPresent && enemyDied) || (!enemyPresent && dungeon.isWalkable(newX, newY))) {
                        hero.setPosition(newX, newY);
                        updateCamera();
                    }

                    // 3. Enemies take turns
                    enemyManager.takeTurns(hero, dungeon);

                    // 4. Check death
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
}

void GameEngine::updateCamera() {
    float halfW = static_cast<float>(VIEW_WIDTH_TILES  * TILE_SIZE) / 2.f;
    float halfH = static_cast<float>(VIEW_HEIGHT_TILES * TILE_SIZE) / 2.f;
    float mapW  = static_cast<float>(dungeon.getWidth()  * TILE_SIZE);
    float mapH  = static_cast<float>(dungeon.getHeight() * TILE_SIZE);

    float cx = static_cast<float>(hero.getX() * TILE_SIZE) + TILE_SIZE / 2.f;
    float cy = static_cast<float>(hero.getY() * TILE_SIZE) + TILE_SIZE / 2.f;
    
    if (cx < halfW) cx = halfW;
    if (cy < halfH) cy = halfH;
    if (cx > mapW - halfW) cx = mapW - halfW;
    if (cy > mapH - halfH) cy = mapH - halfH;
    
    camera.setCenter({cx, cy});
    
    // Also update player sprite position here so it tracks with hero coordinates
    playerSprite.setPosition({
        static_cast<float>(hero.getX() * TILE_SIZE),
        static_cast<float>(hero.getY() * TILE_SIZE)
    });
}

void GameEngine::render() {
    window.clear(sf::Color::Black);

    // Apply world camera
    window.setView(camera);

    mapRenderer.draw(window, dungeon);
    enemyManager.draw(window);
    window.draw(playerSprite);

    // Draw HUD (uses screen-space internally)
    hudRenderer.draw(window, hero, smallPotions, medPotions, largePotions, gameOver);

    // Reapply world camera before display just in case
    window.setView(camera);
    window.display();
}
