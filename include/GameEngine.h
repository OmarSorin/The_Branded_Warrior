#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Character.h"
#include "MapRenderer.h"
#include "EnemyManager.h"
#include "HudRenderer.h"

class GameEngine {
private:
    static constexpr int TILE_SIZE = 16;
    static constexpr int VIEW_WIDTH_TILES = 20;
    static constexpr int VIEW_HEIGHT_TILES = 15;

    // Core SFML 
    sf::RenderWindow window;
    sf::View camera;
    sf::Font hudFont;

    // Game State
    Map dungeon;
    Character hero;
    bool gameOver;

    // Subsystems
    MapRenderer mapRenderer;
    EnemyManager enemyManager;
    HudRenderer hudRenderer;

    // Player Rendering
    sf::Texture texRight, texLeft, texUp, texDown;
    sf::Sprite playerSprite;

    // Potion Inventory
    int smallPotions = 0;
    int medPotions = 0;
    int largePotions = 0;
    Potion potSmall;
    Potion potMed;
    Potion potLarge;

    // Internal Helpers
    void handleInput();
    void updateCamera();
    void render();

public:
    GameEngine();
    
    // Initializes the map, hero, enemies, and assets
    void initialize();
    
    // Starts the main game loop
    void run();
};

#endif // GAMEENGINE_H
