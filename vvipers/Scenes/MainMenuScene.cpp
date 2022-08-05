#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Scenes/UIElements/MenuButton.hpp>
#include <vvipers/Scenes/ArenaScene.hpp>
#include <vvipers/Scenes/OptionsMenuScene.hpp>

namespace VVipers
{

MainMenuScene::MainMenuScene(Game& game) : MenuScene(game) {
    auto size = game.getWindow().getSize();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.25 * size), 0.5 * Vec2(size.x, size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(0.25, 0.25, 0.5, 0.5));
    setMenuView(menuView);

    m_playButton = std::make_unique<MenuButton>();
    m_playButton->setLabel("Play");
    m_playButton->setFont( *game.getFontService().getDefaultFont() );
    addItem(m_playButton.get());

    m_optionsButton = std::make_unique<MenuButton>();
    m_optionsButton->setLabel("Options");
    m_optionsButton->setFont( *game.getFontService().getDefaultFont() );
    addItem(m_optionsButton.get());

    m_quitButton = std::make_unique<MenuButton>();
    m_quitButton->setLabel("Quit");
    m_quitButton->setFont( *game.getFontService().getDefaultFont() );
    addItem(m_quitButton.get());

    setSelectedIndex(0);
    distributeMenuItems();
}

void MainMenuScene::onMenuItemActivation(MenuItem* menuItem){
    if( menuItem == m_playButton.get()){
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Spawn );
        m_transitionTo = std::make_unique<ArenaScene>( getGame());
    }else if( menuItem == m_optionsButton.get()){
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Spawn );
        m_transitionTo = std::make_unique<OptionsMenuScene>( getGame());
    }else if( menuItem == m_quitButton.get()){
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Quit );
    }
}

scene_ptr MainMenuScene::makeTransition(){
    return std::move(m_transitionTo);
}

    
} // namespace VVipers