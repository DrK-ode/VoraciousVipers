#ifndef VVIPERS_GAME_HPP
#define VVIPERS_GAME_HPP

#include <SFML/Graphics.hpp>
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
    std::map<Player*, Viper*> m_players;
};

}  // namespace VVipers

#endif