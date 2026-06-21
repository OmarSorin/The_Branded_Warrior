#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Character.h"
#include "MapRenderer.h"
#include "EnemyManager.h"
#include "HudRenderer.h"
#include "Levelmanager.h"
#include "MessageLog.h"

class GameEngine {
private:
    static constexpr int TILE_SIZE = 16;
    static constexpr int VIEW_WIDTH_TILES = 20;
    static constexpr int VIEW_HEIGHT_TILES = 15;
    static constexpr float EXIT_HINT_SECONDS = 2.5f;

    // Core SFML
    sf::RenderWindow window;
    sf::View camera;
    sf::Font hudFont;

    // Game State
    Map dungeon;
    Character hero;
    bool gameOver;
    int currentLevel = 0;
    bool won = false;

    // defeat all enemies banner shown after bumping a sealed exit.
    bool exitHintActive = false;
    sf::Clock exitHintClock;

    // Subsystems
    MessageLog messageLog;
    Levelmanager levelManager;
    MapRenderer mapRenderer;
    EnemyManager enemyManager;
    HudRenderer hudRenderer;

    // Player Rendering
    sf::Texture texRight, texLeft, texUp, texDown;
    sf::Sprite playerSprite;

    // Potion Inventory
    // int smallPotions = 0;
    // int medPotions = 0;
    // int largePotions = 0;
    Potion potSmall;
    Potion potMed;
    Potion potLarge;

    // Internal Helpers
    void handleInput();
    void updateCamera();
    void render();

    // Builds (or rebuilds) the world for the given level index:
    // regenerates the dungeon, repositions the hero, respawns enemies.
    void loadLevel(int index);

public:
    GameEngine();

    // Initializes the map, hero, enemies, and assets
    void initialize();

    // Starts the main game loop
    void run();
};

#endif // GAMEENGINE_H