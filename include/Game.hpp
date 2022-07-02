#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Viper.hpp"

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