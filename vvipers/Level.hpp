#ifndef VVIPERS_LEVEL_HPP
#define VVIPERS_LEVEL_HPP

#include <string>
#include <vvipers/Collidable.hpp>
#include <SFML/Graphics.hpp>

namespace VVipers {

class Level : public Collidable, public sf::Drawable {
  public:
    Level(std::string name) : m_name(name){ constructLevel(); }
    ~Level();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void updateNodes(){};

  protected:
    virtual void constructLevel();

  private:
    std::string m_name;
    std::vector<sf::Shape*> m_shapes;
};

}  // namespace VVipers

#endif