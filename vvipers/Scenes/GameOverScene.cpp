#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/FlashScreenScene.hpp>
#include <vvipers/Scenes/GameOverScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

GameOverScene::GameOverScene(Game& game,
                             std::vector<std::shared_ptr<const Player>> players)
    : Scene(game), m_players(players) {
    Vec2 size = getGame().getWindow().getSize();
    m_gameOverText.setFont(*getGame().getFontService().getDefaultFont());
    m_gameOverText.setString("Game Over");
    m_gameOverText.setCharacterSize(0.1 * size.y);
    m_gameOverText.setPosition(size / 2);
    auto lbp = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin(Vec2(lbp.left + lbp.width, lbp.top + lbp.height) /
                             2);
    m_gameOverText.setFillColor( game.getColorService().getColor(0));
    m_gameOverText.setOutlineColor(game.getColorService().getColor(1));
    m_gameOverText.setOutlineThickness(0.005 * size.y);

    m_scoreText.setFont(*getGame().getFontService().getDefaultFont());
    m_scoreText.setString(getScoreString(players));
    m_scoreText.setCharacterSize(0.5 * m_gameOverText.getCharacterSize());
    m_scoreText.setPosition(m_gameOverText.getPosition() +
                            Vec2(0, 2 * m_gameOverText.getCharacterSize()));
    auto lbq = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(Vec2(lbq.left + lbq.width, lbq.top + lbq.height) / 2);
    m_scoreText.setFillColor(game.getColorService().getColor(0));
    m_scoreText.setOutlineColor(game.getColorService().getColor(1));
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
                  return lhs->score() > rhs->score();
              });
    std::stringstream ss;
    for (auto p = players.cbegin(); p != players.cend(); ++p) {
        if (p != players.cbegin())
            ss << '\n';
        ss << (*p)->name() << ": " << (*p)->score() << " (Viper level "
           << Player::calculateLevel((*p)->score()) << ")";
    }
    return ss.str();
}

void GameOverScene::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
    target.draw(m_background, states);
    target.draw(m_gameOverText, states);
    target.draw(m_scoreText, states);
}

void GameOverScene::update(Time elapsedTime) {}

void GameOverScene::processEvent(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed: {
            setTransitionState(TransitionState::Quit);
            break;
        }
        case sf::Event::KeyPressed: {
            if ((event.key.code == sf::Keyboard::Return) or
                (event.key.code == sf::Keyboard::Escape))
                setTransitionState(TransitionState::Default);
            break;
        }
        case sf::Event::MouseButtonPressed: {
            setTransitionState(TransitionState::Default);
            break;
        }
        default:
            break;
    }
}

}  // namespace VVipers