#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/FlashScreenScene.hpp>
#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

FlashScreenScene::FlashScreenScene(const Game& game, Time duration)
    : Scene(game), m_timeLeft(duration) {
    Vec2 size = getGame().getWindow().getSize();
    m_text.setFont(*getGame().getFontService().getDefaultFont());
    m_text.setString("Voracious Vipers");
    m_text.setCharacterSize(0.1 * size.y);
    m_text.setPosition(size / 2);
    auto lb = m_text.getLocalBounds();
    m_text.setOrigin(Vec2(lb.left + lb.width, lb.top + lb.height) / 2);
    m_text.setFillColor(sf::Color::Green);
    m_text.setOutlineColor(sf::Color::Red);
    m_text.setOutlineThickness(0.005 * size.y);
}

void FlashScreenScene::draw(sf::RenderTarget& target,
                            sf::RenderStates states) const {
    target.draw(m_text, states);
}

void FlashScreenScene::update(Time elapsedTime) {
    m_timeLeft -= elapsedTime;
    if (m_timeLeft <= seconds(0)) {
        setTransitionState(TransitionState::Default);
    }
}

void FlashScreenScene::processEvent(const sf::Event& event) {
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

}  // namespace VVipers
