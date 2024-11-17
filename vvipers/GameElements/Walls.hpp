#ifndef VVIPERS_GAMEELEMENTS_WALLS_HPP
#define VVIPERS_GAMEELEMENTS_WALLS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <memory>
#include <vvipers/GameElements/GameObject.hpp>
#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/Utilities/Shape.hpp"
#include "vvipers/Utilities/TriangleStripArray.hpp"

namespace VVipers {

class Walls : public GameObject, public sf::Drawable, public CollidingBody {
  public:
    Walls(Vec2 levelSize);
    size_t number_of_segments() const override {return _polygons.size();}
    std::shared_ptr<const Shape> segment_shape(size_t index) const override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  protected:
    virtual void constructLevel();

  private:
    Vec2 _level_size;
    std::vector<TriangleStripArray> _triangle_strips;
    std::vector<std::shared_ptr<Polygon>> _polygons;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEELEMENTS_WALLS_HPP
