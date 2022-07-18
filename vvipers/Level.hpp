#ifndef VVIPERS_LEVEL_HPP
#define VVIPERS_LEVEL_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionBody.hpp>

namespace VVipers {

class Level : public sf::Drawable, public Collidable {
  public:
    Level(std::string name);
    ~Level();
    std::vector<const CollisionBody*> collisionBodies() const override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  protected:
    virtual void constructLevel();

  private:
    std::string m_name;
    std::vector<const CollisionBody*> m_collisionBodies;
};

}  // namespace VVipers

#endif