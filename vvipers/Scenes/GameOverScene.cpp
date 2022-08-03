#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/GameOverScene.hpp>
#include <vvipers/Scenes/FlashScreenScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

GameOverScene::GameOverScene(
    Game& game, std::vector<std::shared_ptr<const Player>> players)
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
    m_scoreText.setString(getScoreString(players));
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

std::string GameOverScene::getScoreString(
    std::vector<std::shared_ptr<const Player>>& players) {
    std::sort(players.begin(), players.end(),
              [](const auto& lhs, const auto& rhs) {
                  return lhs->score() < rhs->score();
              });
    std::stringstream ss;
    for (auto p = players.cbegin(); p != players.cend(); ++p) {
        if (p != players.cbegin())
            ss << '\n';
        ss << (*p)->name() << ": " << (*p)->score() << " (Viper level "
           << Player::calculateLevel((*p)->score()) << ")";
    }
    return std::move(ss.str());
}

void GameOverScene::draw() {
    m_game.getWindow().draw(m_background);
    m_game.getWindow().draw(m_gameOverText);
    m_game.getWindow().draw(m_scoreText);
}

void GameOverScene::update(Time elapsedTime) {}

void GameOverScene::processEvents() {
    sf::Event event;
    while (m_game.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            setTransitionState(TransitionState::Quit);
        } else if (event.type == sf::Event::KeyPressed) {
            setTransitionState(TransitionState::Default);
            break;
        }
    }
}

}  // namespace VVipers