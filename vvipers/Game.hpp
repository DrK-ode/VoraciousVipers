#ifndef VVIPERS_GAME_HPP
#define VVIPERS_GAME_HPP

#include <SFML/Graphics.hpp>

#include <vvipers/Player.hpp>
#include <vvipers/Viper.hpp>

class Game : public sf::Drawable {
  public:
    Game();
    ~Game();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void tick(sf::Time elapsedTime);

  private:
    std::map<Player*,Viper*> m_players;
};

#endif