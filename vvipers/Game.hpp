#ifndef VVIPERS_GAME_HPP
#define VVIPERS_GAME_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <vvipers/Collision.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/Time.hpp>

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
    bool exit() const {return m_exit;}
    void onNotify(const GameEvent* event) override;
    void processEvents();
    void update(Time elapsedTime);

  private:
    void connect(Controller*, Viper*);
    void connect(Viper*, Player*);
    Viper* belongsTo(const Controller*);
    Player* belongsTo(const Viper*);
    void signalExit();

    bool m_exit;
    std::multimap<GameEvent::EventType, const GameEvent*> m_eventsToBeProcessed;
    std::map<const Viper*, Player*> m_mapViperPlayer;
    std::map<const Controller*, Viper*> m_mapControllerViper;
    CollisionDetector m_collisionDetector;
    Level* m_currentLevel;
    std::vector<Controller*> m_controllers;
    std::vector<Player*> m_players;
    std::vector<Viper*> m_vipers;
};

}  // namespace VVipers

#endif