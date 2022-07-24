#ifndef VVIPERS_LEVEL_HPP
#define VVIPERS_LEVEL_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <vector>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/GameObject.hpp>

namespace VVipers {

class RectBody;

class Level : public GameObject, public sf::Drawable, public Collidable {
  public:
    Level(const std::string& name, Vec2 levelSize);
    ~Level();

    enum LevelObject : PartID_t {LevelWall};

    std::vector<const CollisionBody*> collisionBodies() const override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  protected:
    virtual void constructLevel(Vec2 levelSize);

  private:
    std::string m_name;
    std::vector<const RectBody*> m_rects;
};

}  // namespace VVipers

#endif