#ifndef VVIPERS_LEVEL_HPP
#define VVIPERS_LEVEL_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionBody.hpp>

namespace VVipers {

class Level : public sf::Drawable, public Collidable {
  public:
    Level(std::string name)
        : m_name(name), m_collisionBody(sf::PrimitiveType::Quads) {
        constructLevel();
    }
    std::vector<const CollisionBody*> collisionBodies() const override {
        return std::vector<const CollisionBody*>({&m_collisionBody});
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void updateNodes(){};

  protected:
    virtual void constructLevel();

  private:
    std::string m_name;
    CollisionBody m_collisionBody;
};

}  // namespace VVipers

#endif