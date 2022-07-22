#ifndef VVIPERS_GAME_HPP
#define VVIPERS_GAME_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <vvipers/CollisionDetector.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/Time.hpp>
#include <vvipers/GameObject.hpp>

namespace VVipers {

class Controller;
class Level;
class Viper;
class Player;

class Game : public sf::Drawable, public Observer, Observable {
  public:
    Game();
    ~Game();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool exit() const { return m_exit; }
    void onNotify(const GameEvent* event) override;
    void processEvents();
    void update(Time elapsedTime);

  private:
    Controller* addController(Controller* controller);
    void deleteController(Controller* controller);
    Player* addPlayer(const std::string& name, Controller* controller,
                      Viper* viper);
    void deletePlayer(Player* player);
    Viper* addViper(/* Start conditions */);
    void deleteViper(Viper* viper);
    void killViper(Viper* viper);

    Player* findPlayerWith(const Viper*) const;
    Player* findPlayerWith(const Controller*) const;

    void handleCollisions(const CollisionEvent* event);
    void handleSteering(const SteeringEvent* event);
    void handleDestruction( const DestroyEvent* event);

    void signalExit();

    bool m_exit;
    std::multimap<GameEvent::EventType, const GameEvent*> m_eventsToBeProcessed;
    CollisionDetector m_collisionDetector;
    Level* m_currentLevel;
    Vec2 m_mouseMove;
    std::set<Controller*> m_controllers;
    std::set<Player*> m_players;
    std::set<Viper*> m_vipers;
};

}  // namespace VVipers

#endif