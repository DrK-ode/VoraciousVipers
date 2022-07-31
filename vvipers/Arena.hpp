#ifndef VVIPERS_ARENA_HPP
#define VVIPERS_ARENA_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <vvipers/CollisionDetector.hpp>
#include <vvipers/FlyingScore.hpp>
#include <vvipers/Food.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/GameObject.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/PlayerPanel.hpp>
#include <vvipers/Time.hpp>
#include <vvipers/Scene.hpp>
#include <vvipers/Game.hpp>

namespace VVipers {

class Controller;
class Walls;
class Viper;
class Player;

class Arena : public Scene, public Observer {
  public:
    Arena(Game& game);
    ~Arena();
    void draw();
    void onNotify(const GameEvent* event) override;
    void processEvents() override;
    void update(Time elapsedTime) override;

  private:
    Controller* addController(Controller* controller);
    void deleteController(Controller* controller);
    Controller* addMouseController();
    Controller* addKeyboardController();

    Player* addPlayer(const std::string& name, Controller* controller,
                      Viper* viper);
    void deletePlayer(Player* player);
    Viper* addViper(/* Start conditions */);
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

    void processWindowEvents();
    void signalExit();

    sf::View* m_statusBarView;
    sf::View* m_gameView;
    Game& m_game;
    std::multimap<GameEvent::EventType, const GameEvent*> m_eventsToBeProcessed;
    CollisionDetector m_collisionDetector;
    Walls* m_walls;
    std::set<Controller*> m_controllers;
    std::set<Player*> m_players;
    std::set<PlayerPanel*> m_playerPanels;
    std::set<Viper*> m_vipers;
    std::set<Food*> m_food;
    std::set<FlyingScore*> m_flyingScores;
};

}  // namespace VVipers

#endif // VVIPERS_ARENA_HPP
