#ifndef VVIPER_VIPER_HPP
#define VVIPER_VIPER_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <vvipers/Controller.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/ViperGraphics.hpp>
#include <vvipers/ViperPhysics.hpp>

namespace VVipers {

class Viper : public sf::Drawable, Observer {
  public:
    Viper() : m_controller(nullptr) {}
    ~Viper();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    ViperPhysics& getPhysicalViper() { return m_viperPhys; };
    virtual void onNotify(const GameEvent& event) override;
    void setup(const Vec2& from, double angle, const Time& length);
    void update(const Time& elapsedTime);
    void setController(Controller* ctrl);

  private:
    Controller* m_controller;
    ViperPhysics m_viperPhys;
    ViperGraphics m_viperGraph;
};

}  // namespace VVipers
#endif