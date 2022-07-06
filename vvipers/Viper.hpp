#ifndef VVIPER_VIPER_HPP
#define VVIPER_VIPER_HPP

#include <SFML/Graphics/Drawable.hpp>

#include <vvipers/ViperGraphics.hpp>
#include <vvipers/ViperPhysics.hpp>

namespace VVipers {

class Viper : public sf::Drawable {
  public:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    ViperPhysics& getPhysicalViper() { return m_viperPhys; };
    void setup(const Vec2& from, double angle, const Time& length);
    void update(const Time& elapsedTime);

  private:
    ViperPhysics m_viperPhys;
    ViperGraphics m_viperGraph;
};

}  // namespace VVipers
#endif