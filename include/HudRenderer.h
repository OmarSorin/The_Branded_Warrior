#ifndef HUDRENDERER_H
#define HUDRENDERER_H

#include <SFML/Graphics.hpp>
#include "Character.h"

// Owns and draws all screen-space HUD elements:
//   - HP bar (upper-left)
//   - XP bar (below HP bar)
//   - Potion panel (upper-right)  [U] small  [I] medium  [O] large
//   - "YOU DIED" overlay
class HudRenderer {
private:
    //Layout constants
    static constexpr float HP_BAR_MAX_W = 200.f;
    static constexpr float BAR_H        =  20.f;
    static constexpr float HUD_PAD      =   6.f;
    static constexpr float HUD_X        =  16.f;
    static constexpr float HUD_Y        =  16.f;
    static constexpr float XP_BAR_Y     = HUD_Y + BAR_H + HUD_PAD * 2 + 4.f;

    static constexpr float POT_ROW_H   = 30.f;
    static constexpr float POT_ICON_W  = 14.f;
    static constexpr float POT_ICON_H  = 20.f;
    static constexpr float POT_PANEL_W = 120.f;
    static constexpr float POT_HUD_X   = 1280.f - POT_PANEL_W - 16.f;
    static constexpr float POT_HUD_Y   = 16.f;

    //HP bar
    sf::RectangleShape hpBg, hpBar;
    sf::Text           hpText;

    //XP bar
    sf::RectangleShape xpBg, xpBar;
    sf::Text           xpText;

    //Potion panel
    sf::RectangleShape potBg;
    sf::RectangleShape flaskSmall, flaskMed, flaskLarge;
    sf::Text           potLabelSmall, potLabelMed, potLabelLarge;

    //YOU DIED overlay
    sf::RectangleShape deathOverlay;
    sf::Text           deathTitle, deathSub;

    // Exit hint banner
    sf::Text exitHint;

public:
    explicit HudRenderer(const sf::Font& font);

    // Switches to default (screen-space) view, draws everything, doesnt
    // restore the world camera
    void draw(sf::RenderWindow& window,
              const Character&  hero,
              int smallPotions,
              int medPotions,
              int largePotions,
              bool gameOver,
              bool showExitHint);
};

#endif // HUDRENDERER_H