#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "Viper.hpp"

class Game : public sf::Drawable {
  public:
    Game();
    ~Game();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void tick(sf::Time elapsedTime);

  private:
    Viper* m_viper;
};

#endif