#include "GameEngine.h"
#include "GameException.h"
#include <iostream>

int main() {
    try {
        GameEngine game;
        game.initialize();
        game.run();
    } catch (const GameException &e) {
        std::cerr << "Game error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
    return 0;

}
