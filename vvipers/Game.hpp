#ifndef VVIPERS_GAME_HPP
#define VVIPERS_GAME_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vvipers/CollisionDetector.hpp>
#include <vvipers/Food.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/GameObject.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/Time.hpp>

namespace VVipers {

class Controller;
class Walls;
class Viper;
class Player;

class Game : public sf::RenderWindow, public Observer, Observable {
  public:
    Game(Vec2 windowSize);
    ~Game();
    void draw();
    bool exit() const { return m_exit; }
    void onNotify(const GameEvent* event) override;
    void processEvents();
    void update(Time elapsedTime);

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

    Player* findPlayerWith(const Viper*) const;
    Player* findPlayerWith(const Controller*) const;

    Vec2 findFreeRect(Vec2 rectSize) const {
      return findFreeRect( rectSize, sf::Rect<double>(0,0,getSize().x, getSize().y ) );
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

    bool m_exit;
    std::multimap<GameEvent::EventType, const GameEvent*> m_eventsToBeProcessed;
    CollisionDetector m_collisionDetector;
    Walls* m_walls;
    Vec2 m_mouseMove;
    std::set<Controller*> m_controllers;
    std::set<Player*> m_players;
    std::set<Viper*> m_vipers;
    std::set<Food*> m_food;
};

}  // namespace VVipers

#endif