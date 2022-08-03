#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Scenes/UIElements/MenuButton.hpp>
#include <vvipers/Scenes/ArenaScene.hpp>

namespace VVipers
{

MainMenuScene::MainMenuScene(Game& game, sf::View view): MenuScene(game,view), m_game(game) {
    m_playButton = std::make_unique<MenuButton>();
    m_playButton->setLabel("Play");
    m_playButton->setFont( *game.getFontService().getDefaultFont() );
    addItem(m_playButton.get());

    m_quitButton = std::make_unique<MenuButton>();
    m_quitButton->setLabel("Quit");
    m_quitButton->setFont( *game.getFontService().getDefaultFont() );
    addItem(m_quitButton.get());

    setSelectedIndex(0);
    setLayoutOrientation(Horizontal);
    m_playButton->setFontRatio(0.1);
    m_quitButton->setFontRatio(0.1);
    distributeMenuItems();
}

void MainMenuScene::onMenuItemActivation(MenuItem* menuItem){
    if( menuItem == m_playButton.get()){
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Spawn );
    }else if( menuItem == m_quitButton.get()){
        setSceneState(SceneState::Paused);
        setTransitionState(TransitionState::Quit );
    }
}

scene_ptr MainMenuScene::makeTransition(){
    return scene_ptr( new ArenaScene(m_game) );
}

    
} // namespace VVipers