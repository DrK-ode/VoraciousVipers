#include <vvipers/Scenes/OptionsMenuScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>

namespace VVipers
{

OptionsMenuScene::OptionsMenuScene(Game& game) : MenuScene(game) {
    auto size = game.getWindow().getSize();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.25 * size), 0.5 * Vec2(size.x, size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(0.25, 0.25, 0.5, 0.5));
    setMenuView(menuView);

    m_backButton = std::make_unique<MenuButton>();
    m_backButton->setLabel("Back");
    m_backButton->setFont( *game.getFontService().getDefaultFont() );
    addItem(m_backButton.get());

    setSelectedIndex(0);
    distributeMenuItems();
    setTransparent(true);
}

void OptionsMenuScene::onMenuItemActivation(MenuItem* menuItem){
    if( menuItem == m_backButton.get()){
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Return );
    }
}

} // namespace VVipers