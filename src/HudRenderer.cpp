#include "HudRenderer.h"
#include "Messagelog.h"
#include <string>

HudRenderer::HudRenderer(const sf::Font& font)
    : hpText(font),
      xpText(font),
      potLabelSmall(font),
      potLabelMed(font),
      potLabelLarge(font),
      deathTitle(font),
      deathSub(font),
      exitHint(font),
      floorText(font),
      victoryTitle(font),
      victorySub(font),
      logLine(font) {
    // HP bar setup
    hpBg.setSize({HP_BAR_MAX_W + HUD_PAD * 2, BAR_H + HUD_PAD * 2});
    hpBg.setFillColor(sf::Color(0, 0, 0, 180));
    hpBg.setOutlineColor(sf::Color(160, 30, 30));
    hpBg.setOutlineThickness(2.f);
    hpBg.setPosition({HUD_X, HUD_Y});

    hpBar.setFillColor(sf::Color(200, 30, 30));
    hpBar.setPosition({HUD_X + HUD_PAD, HUD_Y + HUD_PAD});

    hpBar.setPosition({HUD_X + HUD_PAD, HUD_Y + HUD_PAD});
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);

    // XP bar setup
    xpBg.setSize({HP_BAR_MAX_W + HUD_PAD * 2, BAR_H + HUD_PAD * 2});
    xpBg.setFillColor(sf::Color(0, 0, 0, 180));
    xpBg.setOutlineColor(sf::Color(30, 80, 160));
    xpBg.setOutlineThickness(2.f);
    xpBg.setPosition({HUD_X, XP_BAR_Y});

    xpBar.setFillColor(sf::Color(50, 120, 220));
    xpBar.setPosition({HUD_X + HUD_PAD, XP_BAR_Y + HUD_PAD});
    xpText.setCharacterSize(20);
    xpText.setFillColor(sf::Color::White);

    // Potion panel setup
    potBg.setSize({POT_PANEL_W, POT_ROW_H * 3 + 8.f});
    potBg.setFillColor(sf::Color(0, 0, 0, 160));
    potBg.setOutlineColor(sf::Color(100, 80, 40));
    potBg.setOutlineThickness(1.5f);
    potBg.setPosition({POT_HUD_X - 4.f, POT_HUD_Y - 4.f});

    flaskSmall.setSize({POT_ICON_W, POT_ICON_H});
    flaskSmall.setFillColor(sf::Color(50, 200, 80));

    flaskMed.setSize({POT_ICON_W, POT_ICON_H});
    flaskMed.setFillColor(sf::Color(50, 120, 220));

    flaskLarge.setSize({POT_ICON_W, POT_ICON_H});
    flaskLarge.setFillColor(sf::Color(180, 60, 220));

    potLabelSmall.setCharacterSize(16);
    potLabelSmall.setFillColor(sf::Color(180, 255, 180));

    potLabelMed.setCharacterSize(16);
    potLabelMed.setFillColor(sf::Color(150, 190, 255));

    potLabelLarge.setCharacterSize(16);
    potLabelLarge.setFillColor(sf::Color(220, 160, 255));

    // YOU DIED overlay setup
    deathOverlay.setSize({1280.f, 720.f});
    deathOverlay.setFillColor(sf::Color(0, 0, 0, 180));
    deathTitle.setString("YOU DIED! :<");
    deathTitle.setCharacterSize(96);
    deathTitle.setFillColor(sf::Color(180, 20, 20));
    deathTitle.setStyle(sf::Text::Bold);
    auto dtBounds = deathTitle.getLocalBounds();
    deathTitle.setOrigin({dtBounds.position.x + dtBounds.size.x / 2.f,
                          dtBounds.position.y + dtBounds.size.y / 2.f});
    deathTitle.setPosition({640.f, 310.f});
    deathSub.setString("Press ESC to quit");
    deathSub.setCharacterSize(28);
    deathSub.setFillColor(sf::Color(200, 200, 200));
    auto dsBounds = deathSub.getLocalBounds();
    deathSub.setOrigin({dsBounds.position.x + dsBounds.size.x / 2.f,
                        dsBounds.position.y + dsBounds.size.y / 2.f});
    deathSub.setPosition({640.f, 420.f});

    // Exit hint banner setup
    exitHint.setString("You must defeat all enemies before advancing");
    exitHint.setCharacterSize(24);
    exitHint.setFillColor(sf::Color(255, 230, 150));
    exitHint.setStyle(sf::Text::Bold);
    exitHint.setOutlineColor(sf::Color(0, 0, 0, 200));
    exitHint.setOutlineThickness(2.f);
    auto ehBounds = exitHint.getLocalBounds();
    exitHint.setOrigin({ehBounds.position.x + ehBounds.size.x / 2.f,
                        ehBounds.position.y + ehBounds.size.y / 2.f});
    exitHint.setPosition({640.f, 660.f});

    // Floor indicator setup
    floorText.setCharacterSize(22);
    floorText.setFillColor(sf::Color(230, 220, 200));
    floorText.setStyle(sf::Text::Bold);
    floorText.setOutlineColor(sf::Color(0, 0, 0, 200));
    floorText.setOutlineThickness(2.f);

    // VICTORY overlay setup
    victoryTitle.setString("YOU ESCAPED! :>");
    victoryTitle.setCharacterSize(96);
    victoryTitle.setFillColor(sf::Color(220, 190, 60));
    victoryTitle.setStyle(sf::Text::Bold);
    auto vtBounds = victoryTitle.getLocalBounds();
    victoryTitle.setOrigin({vtBounds.position.x + vtBounds.size.x / 2.f,
                            vtBounds.position.y + vtBounds.size.y / 2.f});
    victoryTitle.setPosition({640.f, 310.f});

    // DEAD setup
    victorySub.setString("Press ESC to quit");
    victorySub.setCharacterSize(28);
    victorySub.setFillColor(sf::Color(200, 200, 200));
    auto vsBounds = victorySub.getLocalBounds();
    victorySub.setOrigin({vsBounds.position.x + vsBounds.size.x / 2.f,
                          vsBounds.position.y + vsBounds.size.y / 2.f});
    victorySub.setPosition({640.f, 420.f});

    // Combat log line setup
    logLine.setCharacterSize(16);
    logLine.setOutlineColor(sf::Color(0, 0, 0, 200));
    logLine.setOutlineThickness(1.5f);
}

void HudRenderer::draw(sf::RenderWindow& window,
                       const Character&  hero,
                       int smallPotions,
                       int medPotions,
                       int largePotions,
                       bool gameOver,
                       bool showExitHint,
                       bool won,
                       int currentFloor,
                       int totalFloors,
                       const MessageLog& log) {
    // Reset to screen-space view for HUD rendering
    window.setView(window.getDefaultView());

    // Draw HP bar
    float hpRatio = static_cast<float>(hero.getHp()) / static_cast<float>(hero.getMaxHp());
    if (hpRatio < 0.f) hpRatio = 0.f;
    hpBar.setSize({HP_BAR_MAX_W * hpRatio, BAR_H});

    hpText.setString(std::to_string(hero.getHp()) + "/" + std::to_string(hero.getMaxHp()));
    hpText.setPosition({HUD_X + HP_BAR_MAX_W + HUD_PAD * 2 + 6.f, HUD_Y});

    window.draw(hpBg);
    window.draw(hpBar);
    window.draw(hpText);

    // Draw XP bar
    float xpRatio = static_cast<float>(hero.getXp()) / static_cast<float>(hero.getXpForNextLevel());
    if (xpRatio < 0.f) xpRatio = 0.f;
    if (xpRatio > 1.f) xpRatio = 1.f;
    xpBar.setSize({HP_BAR_MAX_W * xpRatio, BAR_H});

    xpText.setString("Lv" + std::to_string(hero.getLevel()) + "  "
                     + std::to_string(hero.getXp()) + "/" + std::to_string(hero.getXpForNextLevel()));
    xpText.setPosition({HUD_X + HP_BAR_MAX_W + HUD_PAD * 2 + 6.f, XP_BAR_Y});

    window.draw(xpBg);
    window.draw(xpBar);
    window.draw(xpText);

    // Draw Potion Panel
    window.draw(potBg);
    const float textOffX = POT_ICON_W + 6.f;

    flaskSmall.setPosition({POT_HUD_X, POT_HUD_Y});
    potLabelSmall.setString("[U] x" + std::to_string(smallPotions) + "  +25");
    potLabelSmall.setPosition({POT_HUD_X + textOffX, POT_HUD_Y});
    window.draw(flaskSmall);
    window.draw(potLabelSmall);

    flaskMed.setPosition({POT_HUD_X, POT_HUD_Y + POT_ROW_H});
    potLabelMed.setString("[I] x" + std::to_string(medPotions) + "  +50");
    potLabelMed.setPosition({POT_HUD_X + textOffX, POT_HUD_Y + POT_ROW_H});
    window.draw(flaskMed);
    window.draw(potLabelMed);

    flaskLarge.setPosition({POT_HUD_X, POT_HUD_Y + POT_ROW_H * 2});
    potLabelLarge.setString("[O] x" + std::to_string(largePotions) + "  +100");
    potLabelLarge.setPosition({POT_HUD_X + textOffX, POT_HUD_Y + POT_ROW_H * 2});
    window.draw(flaskLarge);
    window.draw(potLabelLarge);

    // Draw floor indicator
    floorText.setString("Floor " + std::to_string(currentFloor) + "/"
                        + std::to_string(totalFloors));
    {
        auto fb = floorText.getLocalBounds();
        floorText.setOrigin({fb.position.x + fb.size.x / 2.f, fb.position.y});
        floorText.setPosition({640.f, 12.f});
    }
    window.draw(floorText);

    // Draw combat log (bottom-left, oldest at top)
    {
        const auto& entries = log.getEntries();
        const float lineH = 20.f;
        const float startY = 700.f - static_cast<float>(entries.size()) * lineH;
        int i = 0;
        for (const auto& entry : entries) {
            switch (entry.getType()) {
            case MessageType::Dealt: logLine.setFillColor(sf::Color(120, 200, 120)); break;
            case MessageType::Taken: logLine.setFillColor(sf::Color(220,  90,  80)); break;
            case MessageType::Xp:    logLine.setFillColor(sf::Color(225, 195,  65)); break;
            case MessageType::Info:  logLine.setFillColor(sf::Color(200, 200, 200)); break;
            }
            logLine.setString(entry.getText());
            logLine.setPosition({16.f, startY + static_cast<float>(i) * lineH});
            window.draw(logLine);
            ++i;
        }
    }

    // Draw exit hint banner
    if (showExitHint) {
        window.draw(exitHint);
    }

    // Draw YOU DIED overlay
    if (gameOver) {
        window.draw(deathOverlay);
        window.draw(deathTitle);
        window.draw(deathSub);
    }

    // Draw VICTORY overlay
    if (won) {
        window.draw(deathOverlay); // reuse the dim backdrop
        window.draw(victoryTitle);
        window.draw(victorySub);
    }
}