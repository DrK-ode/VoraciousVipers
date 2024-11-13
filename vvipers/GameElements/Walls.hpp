#ifndef VVIPERS_GAMEELEMENTS_WALLS_HPP
#define VVIPERS_GAMEELEMENTS_WALLS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <memory>
#include <vvipers/GameElements/GameObject.hpp>
#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/Collisions/CollidingSegment.hpp"

namespace VVipers {

class Walls : public GameObject, public sf::Drawable, public CollidingBody {
  public:
    Walls(Vec2 levelSize);
    std::vector<std::shared_ptr<const CollidingSegment>> colliding_segments() const override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  protected:
    virtual void constructLevel();

  private:
    Vec2 _level_size;
    std::vector<std::vector<sf::Vertex>> _polygons;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEELEMENTS_WALLS_HPP
