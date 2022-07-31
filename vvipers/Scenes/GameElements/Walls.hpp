#ifndef VVIPERS_WALLS_HPP
#define VVIPERS_WALLS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <vector>
#include <vvipers/Scenes/Collision/Collidable.hpp>
#include <vvipers/Scenes/Collision/CollisionBody.hpp>
#include <vvipers/Scenes/GameElements/GameObject.hpp>

namespace VVipers {

class ConvexBody;

class Walls : public GameObject, public sf::Drawable, public Collidable {
  public:
    Walls(Vec2 levelSize);
    ~Walls();

    std::vector<const CollisionBody*> collisionBodies() const override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  protected:
    virtual void constructLevel();

  private:
    Vec2 m_levelSize;
    std::vector<const ConvexBody*> m_rects;
};

}  // namespace VVipers

#endif // VVIPERS_WALLS_HPP
