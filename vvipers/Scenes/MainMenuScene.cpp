#include <memory>
#include <vvipers/Scenes/ArenaScene.hpp>
#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Scenes/OptionsMenuScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>

namespace VVipers {

MainMenuScene::MainMenuScene(Game& game) : MenuScene(game) {
    auto size = game.getWindow().getSize();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.25 * size), 0.5 * Vec2(size.x, size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(0.25, 0.25, 0.5, 0.5));
    setMenuView(menuView);

    m_playButton = std::make_unique<MenuButton>();
    m_playButton->setLabel("Play");
    m_playButton->setFont(*game.getFontService().getDefaultFont());
    addItem(m_playButton.get());

    m_optionsButton = std::make_unique<MenuButton>();
    m_optionsButton->setLabel("Options");
    m_optionsButton->setFont(*game.getFontService().getDefaultFont());
    addItem(m_optionsButton.get());

    m_quitButton = std::make_unique<MenuButton>();
    m_quitButton->setLabel("Quit");
    m_quitButton->setFont(*game.getFontService().getDefaultFont());
    addItem(m_quitButton.get());

    distributeMenuItems();
    setSelectedIndex(0);
    setColors(sf::Color::Transparent, game.getColorService().getColor(0),
              game.getColorService().getColor(1));
}

void MainMenuScene::onMenuItemActivation(MenuItem* menuItem) {
    if (menuItem == m_playButton.get()) {
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Spawn);
        m_transitionTo = std::make_shared<ArenaScene>(getGame());
    } else if (menuItem == m_optionsButton.get()) {
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Spawn);
        m_transitionTo = std::make_shared<OptionsMenuScene>(getGame());
    } else if (menuItem == m_quitButton.get()) {
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Quit);
    }
}

std::shared_ptr<Scene> MainMenuScene::makeTransition() { return m_transitionTo; }

}  // namespace VVipers