#ifndef VVIPERS_GAMEELEMENTS_WALLS_HPP
#define VVIPERS_GAMEELEMENTS_WALLS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <memory>
#include <vvipers/GameElements/GameObject.hpp>
#include "vvipers/Collisions/CollidingBody.hpp"

namespace VVipers {

class Walls : public GameObject, public sf::Drawable, public CollidingBody {
  public:
    Walls(Vec2 levelSize);
    size_t number_of_body_parts() const override {return _polygons.size();}
    std::shared_ptr<const Shape> body_part_shape(size_t index) const override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  protected:
    virtual void constructLevel();

  private:
    Vec2 _level_size;
    std::vector<std::shared_ptr<Polygon>> _polygons;
    std::vector<std::vector<sf::Vertex>> _vertex_vectors;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEELEMENTS_WALLS_HPP
