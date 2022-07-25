#ifndef VVIPERS_LEVEL_HPP
#define VVIPERS_LEVEL_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <vector>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/GameObject.hpp>

namespace VVipers {

class ConvexBody;

class Level : public GameObject, public sf::Drawable, public Collidable {
  public:
    Level(const std::string& name, Vec2 levelSize);
    ~Level();

    enum LevelObject : PartID_t {LevelWall, PlacementTest};

    std::vector<const CollisionBody*> collisionBodies() const override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    Vec2 findEmptyArea( Vec2 rectSize ) const;

  protected:
    virtual void constructLevel();

  private:
    std::string m_name;
    Vec2 m_levelSize;
    std::vector<const ConvexBody*> m_rects;
};

}  // namespace VVipers

#endif