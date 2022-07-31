#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/GameOverScreen.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

GameOverScreen::GameOverScreen(Game& game, std::vector<std::shared_ptr<const Player>> players)
    : m_game(game), m_players(players) {
    Vec2 size = m_game.getWindow().getSize();
    m_gameOverText.setFont(*m_game.getFontService().getDefaultFont());
    m_gameOverText.setString("Game Over");
    m_gameOverText.setCharacterSize(0.1 * size.y);
    m_gameOverText.setPosition(size / 2);
    auto lbp = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin(Vec2(lbp.left + lbp.width, lbp.top + lbp.height) /
                             2);
    m_gameOverText.setFillColor(sf::Color::Green);
    m_gameOverText.setOutlineColor(sf::Color::Red);
    m_gameOverText.setOutlineThickness(0.005 * size.y);

    m_scoreText.setFont(*m_game.getFontService().getDefaultFont());
    m_scoreText.setString("Scores:");
    m_scoreText.setCharacterSize(0.5 * m_gameOverText.getCharacterSize());
    m_scoreText.setPosition(m_gameOverText.getPosition() +
                            Vec2(0, m_gameOverText.getCharacterSize()));
    auto lbq = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(Vec2(lbq.left + lbq.width, lbq.top + lbq.height) / 2);
    m_scoreText.setFillColor(sf::Color::Green);
    m_scoreText.setOutlineColor(sf::Color::Red);
    m_scoreText.setOutlineThickness(0.5 * m_gameOverText.getOutlineThickness());

    m_background.setPosition(0, 0);
    m_background.setSize(size);
    m_background.setFillColor(sf::Color(0, 0, 0, 0x80));

    setTransparent(true);
}

void GameOverScreen::draw() {
    m_game.getWindow().draw(m_background);
    m_game.getWindow().draw(m_gameOverText);
    m_game.getWindow().draw(m_scoreText);
}

scene_ptr GameOverScreen::makeTransition() {
    // Setup state for next time the pause screen is invoked
    return scene_ptr(nullptr);
}

void GameOverScreen::update(Time elapsedTime) {}

void GameOverScreen::processEvents() {
    sf::Event event;
    while (m_game.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            setTransitionState(TransitionState::Quit);
        } else if (event.type == sf::Event::KeyPressed) {
            setTransitionState(TransitionState::Return);
            break;
        }
    }
}

}  // namespace VVipers