#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/PauseScreen.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

PauseScreen::PauseScreen(Game& game) : m_game(game) {
    Vec2 size = m_game.getWindow().getSize();
    m_text.setFont(*m_game.getFontService().getDefaultFont());
    m_text.setString("Pause");
    m_text.setCharacterSize(0.1 * size.y);
    m_text.setPosition(size / 2);
    auto lb = m_text.getLocalBounds();
    m_text.setOrigin(Vec2(lb.left + lb.width, lb.top + lb.height) / 2);
    m_text.setFillColor(sf::Color::Green);
    m_text.setOutlineColor(sf::Color::Red);
    m_text.setOutlineThickness(0.005 * size.y);

    m_background.setPosition(0, 0);
    m_background.setSize(size);
    m_background.setFillColor(sf::Color(0, 0, 0, 0x80));

    setTransparent(true);
}

void PauseScreen::draw() {
    m_game.getWindow().draw(m_background);
    m_game.getWindow().draw(m_text);
}

scene_ptr PauseScreen::makeTransition(){
    // Setup state for next time the pause screen is invoked
    setTransitionState( TransitionState::Continue);
    return scene_ptr(nullptr);
}

void PauseScreen::update(Time elapsedTime) {}

void PauseScreen::processEvents() {
    sf::Event event;
    while (m_game.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            setTransitionState(TransitionState::Quit);
        } else if (event.type == sf::Event::KeyPressed) {
            setTransitionState(TransitionState::Return);
        }
    }
}

}  // namespace VVipers