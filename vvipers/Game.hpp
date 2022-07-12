#ifndef VVIPERS_GAME_HPP
#define VVIPERS_GAME_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <vvipers/CollisionDetector.hpp>
#include <vvipers/GameMaster.hpp>
#include <vvipers/Player.hpp>
#include <vvipers/Time.hpp>
#include <vvipers/Viper.hpp>

namespace VVipers {

class Game : public sf::Drawable {
  public:
    Game();
    ~Game();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(Time elapsedTime);

  private:
    GameMaster m_GM;
    CollisionDetector m_collisionDetector;
    std::vector<Controller*> m_controllers;
    std::vector<Player*> m_players;
    std::vector<Viper*> m_vipers;
};

}  // namespace VVipers

#endif