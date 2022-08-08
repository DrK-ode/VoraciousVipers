#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/PauseScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

PauseScene::PauseScene(Game& game) : MenuScene(game) {
    Vec2 size = game.getWindow().getSize();

    m_background.setSize(size);
    m_background.setFillColor(sf::Color(0,0,0,0x80));

    // Center and size in original coordinates
    Vec2 menuRelSize(0.7, 0.2);
    sf::View menuView(0.5 * menuRelSize * size, menuRelSize * size);
    // Relative position and size in screen coordinates
    menuView.setViewport(
        sf::FloatRect(0.5 * (Vec2(1, 1) - menuRelSize), menuRelSize));
    setMenuView(menuView);

    m_continueButton = std::make_unique<MenuButton>();
    m_continueButton->setLabel("Continue");
    m_continueButton->setFont(*game.getFontService().getDefaultFont());
    addItem(m_continueButton.get());

    m_quitButton = std::make_unique<MenuButton>();
    m_quitButton->setLabel("Quit to main menu");
    m_quitButton->setFont(*game.getFontService().getDefaultFont());
    addItem(m_quitButton.get());

    setSelectedIndex(0);
    distributeMenuItems();
    setColors(sf::Color::Transparent, game.getColorService().getColor(0),
              game.getColorService().getColor(1));

    setTransparent(true);
}

void PauseScene::drawBackground(sf::RenderTarget& target,
                                sf::RenderStates states) const {
    target.draw(m_background, states);
}

void PauseScene::onMenuItemActivation(MenuItem* menuItem) {
    if (menuItem == m_continueButton.get()) {
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Return);
    } else if (menuItem == m_quitButton.get()) {
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Default);
    }
}

}  // namespace VVipers