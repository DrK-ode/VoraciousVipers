#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/PauseScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

PauseScene::PauseScene(const Game& game) : Scene(game) {
    Vec2 size = getGame().getWindow().getSize();
    m_pauseText.setFont(*getGame().getFontService().getDefaultFont());
    m_pauseText.setString("Pause");
    m_pauseText.setCharacterSize(0.1 * size.y);
    m_pauseText.setPosition(size / 2);
    auto lbp = m_pauseText.getLocalBounds();
    m_pauseText.setOrigin(Vec2(lbp.left + lbp.width, lbp.top + lbp.height) / 2);
    m_pauseText.setFillColor(sf::Color::Green);
    m_pauseText.setOutlineColor(sf::Color::Red);
    m_pauseText.setOutlineThickness(0.005 * size.y);

    m_quitText.setFont(*getGame().getFontService().getDefaultFont());
    m_quitText.setString("(Press 'q' to return to main menu)");
    m_quitText.setCharacterSize(0.5 * m_pauseText.getCharacterSize());
    m_quitText.setPosition(m_pauseText.getPosition() +
                           Vec2(0, m_pauseText.getCharacterSize()));
    auto lbq = m_quitText.getLocalBounds();
    m_quitText.setOrigin(Vec2(lbq.left + lbq.width, lbq.top + lbq.height) / 2);
    m_quitText.setFillColor(sf::Color::Green);
    m_quitText.setOutlineColor(sf::Color::Red);
    m_quitText.setOutlineThickness(0.5 * m_pauseText.getOutlineThickness());

    m_background.setPosition(0, 0);
    m_background.setSize(size);
    m_background.setFillColor(sf::Color(0, 0, 0, 0x80));

    setTransparent(true);
}

void PauseScene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_background, states);
    target.draw(m_pauseText, states);
    target.draw(m_quitText, states);
}

scene_ptr PauseScene::makeTransition() {
    // Setup for reuse
    setTransitionState(TransitionState::Continue);
    return scene_ptr();
}

void PauseScene::processEvent(const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        setTransitionState(TransitionState::Quit);
    } else if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Q: {
                setTransitionState(TransitionState::Default);
                break;
            }
            default: {
                setTransitionState(TransitionState::Return);
                break;
            }
        }
    }
}

}  // namespace VVipers