#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/FlashScreenScene.hpp>
#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

FlashScreenScene::FlashScreenScene(Game& game, Time duration)
    : m_game(game), m_timeLeft(duration) {
    Vec2 size = m_game.getWindow().getSize();
    m_text.setFont(*m_game.getFontService().getDefaultFont());
    m_text.setString("Voracious Vipers");
    m_text.setCharacterSize(0.1 * size.y);
    m_text.setPosition(size / 2);
    auto lb = m_text.getLocalBounds();
    m_text.setOrigin(Vec2(lb.left + lb.width, lb.top + lb.height) / 2);
    m_text.setFillColor(sf::Color::Green);
    m_text.setOutlineColor(sf::Color::Red);
    m_text.setOutlineThickness(0.005 * size.y);
}

void FlashScreenScene::draw() { m_game.getWindow().draw(m_text); }

void FlashScreenScene::update(Time elapsedTime) {
    m_timeLeft -= elapsedTime;
    if (m_timeLeft <= seconds(0)) {
        setTransitionState(TransitionState::Default);
    }
}

void FlashScreenScene::processEvents() {
    sf::Event event;
    while (m_game.getWindow().pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                setTransitionState(TransitionState::Quit);
                break;
            }
            case sf::Event::KeyPressed:
            case sf::Event::MouseButtonPressed: {
                setTransitionState(TransitionState::Default);
                break;
            }
            default:
                break;
        }
    }
}

}  // namespace VVipers
