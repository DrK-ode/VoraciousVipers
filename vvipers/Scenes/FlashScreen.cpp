#include <SFML/Window/Event.hpp>
#include <vvipers/Scenes/Arena.hpp>
#include <vvipers/Scenes/FlashScreen.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

FlashScreen::FlashScreen(Game& game, Time duration)
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
    m_text.setOutlineThickness(0.005*size.y);
}

void FlashScreen::draw() { m_game.getWindow().draw(m_text); }

void FlashScreen::update(Time elapsedTime) {
    m_timeLeft -= elapsedTime;
    if (m_timeLeft <= seconds(0)) {
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Replace);
    }
}

std::shared_ptr<Scene> FlashScreen::makeTransition() {
    return std::shared_ptr<Scene>(new Arena(m_game));
}

void FlashScreen::processEvents() {
    sf::Event event;
    while (m_game.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            setTransitionState(TransitionState::Quit);
        } else if (event.type == sf::Event::KeyPressed) {
            setTransitionState(TransitionState::Replace);
        }
    }
}

}  // namespace VVipers
