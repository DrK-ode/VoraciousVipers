#ifndef VVIPERS_ARENA_HPP
#define VVIPERS_ARENA_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <vvipers/Scenes/Collision/CollisionDetector.hpp>
#include <vvipers/Scenes/GameElements/FlyingScore.hpp>
#include <vvipers/Scenes/GameElements/Food.hpp>
#include <vvipers/Scenes/GameElements/GameEvent.hpp>
#include <vvipers/Scenes/GameElements/GameObject.hpp>
#include <vvipers/Scenes/GameElements/Observer.hpp>
#include <vvipers/Scenes/GameElements/PlayerPanel.hpp>
#include <vvipers/Scenes/GameElements/Controller.hpp>
#include <vvipers/Scenes/GameElements/Viper.hpp>
#include <vvipers/Scenes/GameElements/Player.hpp>
#include <vvipers/Scenes/GameElements/Walls.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Scenes/Scene.hpp>
#include <vvipers/Engine/Game.hpp>

namespace VVipers {

class Arena : public Scene, public Observer {
  public:
    Arena(Game& game);
    ~Arena();
    void draw();
    void onNotify(const GameEvent* event) override;
    void processEvents() override;
    void update(Time elapsedTime) override;
    std::shared_ptr<Scene> makeTransition() override;

  private:
    controller_ptr addController(controller_ptr controller);
    void deleteController(controller_ptr controller);
    controller_ptr createMouseController();
    controller_ptr createKeyboardController();

    player_ptr addPlayer(const std::string& name, controller_ptr controller,
                      viper_ptr viper);
    void deletePlayer(player_ptr player);
    viper_ptr addViper(/* Start conditions */);
    void deleteViper(Viper* viper);
    void killViper(Viper* viper);
    void addFood(Vec2 position, double diameter);
    void eatFood(Viper*, Food*);
    void deleteFood(Food* food);

    PlayerPanel* findPlayerPanel(const Player* player) const;
    Player* findPlayerWith(const Viper*) const;
    Player* findPlayerWith(const Controller*) const;

    Vec2 findFreeRect(Vec2 rectSize) const {
        return findFreeRect(rectSize,
                            sf::Rect<double>(0, 0, m_gameView->getSize().x,
                                             m_gameView->getSize().y));
    }
    Vec2 findFreeRect(Vec2 rectSize, sf::Rect<double> limits) const;
    void dispenseFood();

    void handleCollision(const Colliders& c);
    void handleCollisions();
    void handleSteering();
    void handleDestruction(const DestroyEvent* event);
    void handleObjectUpdates(Time elapsedTime);
    void checkForGameOver();

    void processWindowEvents();

    sf::View* m_statusBarView;
    sf::View* m_gameView;
    Game& m_game;
    std::shared_ptr<Scene> m_transitionScene;
    std::shared_ptr<Scene> m_pauseScene;
    std::multimap<GameEvent::EventType, const GameEvent*> m_eventsToBeProcessed;
    CollisionDetector m_collisionDetector;
    Walls* m_walls;
    // Arena and Player has joint ownership of controllers and vipers
    std::set<controller_ptr> m_controllers;
    std::set<viper_ptr> m_vipers;
    std::set<player_ptr> m_players;
    std::set<PlayerPanel*> m_playerPanels;
    std::set<Food*> m_food;
    std::set<FlyingScore*> m_flyingScores;
};

}  // namespace VVipers

#endif // VVIPERS_ARENA_HPP
