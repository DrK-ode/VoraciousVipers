#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>
#include <vvipers/GameElements/Walls.hpp>
#include <vvipers/Utilities/VVMath.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/Utilities/TriangleStripArray.hpp"
#include "vvipers/Utilities/Vec2.hpp"

namespace VVipers {

Walls::Walls(Vec2 levelSize) : CollidingBody("Walls"), _level_size(levelSize) {
    constructLevel();
}

std::shared_ptr<const Shape> Walls::body_part_shape(size_t index) const {
    if (index >= number_of_body_parts())
        throw std::runtime_error("Requested body part index is too large.");
    return _polygons[index];
}

void Walls::constructLevel() {
    const double width = _level_size.x;
    const double height = _level_size.y;
    const double edge = 5;
    auto color = sf::Color(0x80, 0x80, 0x80, 255);

    TriangleStripArray top;
    top.vertices.emplace_back(Vec2(0, 0), color);
    top.vertices.emplace_back(Vec2(width, 0), color);
    top.vertices.emplace_back(Vec2(0, edge), color);
    top.vertices.emplace_back(Vec2(width, edge), color);
    TriangleStripArray bottom;
    bottom.vertices.emplace_back(Vec2(0, height - edge), color);
    bottom.vertices.emplace_back(Vec2(width, height - edge), color);
    bottom.vertices.emplace_back(Vec2(0, height), color);
    bottom.vertices.emplace_back(Vec2(width, height), color);
    TriangleStripArray left;
    left.vertices.emplace_back(Vec2(0, edge), color);
    left.vertices.emplace_back(Vec2(edge, edge), color);
    left.vertices.emplace_back(Vec2(0, height - edge), color);
    left.vertices.emplace_back(Vec2(edge, height - edge), color);
    TriangleStripArray right;
    right.vertices.emplace_back(Vec2(width - edge, edge), color);
    right.vertices.emplace_back(Vec2(width, edge), color);
    right.vertices.emplace_back(Vec2(width - edge, height - edge), color);
    right.vertices.emplace_back(Vec2(width, height - edge), color);
    TriangleStripArray middle;
    middle.vertices.emplace_back(Vec2(width * 0.25, height * 0.375), color);
    middle.vertices.emplace_back(Vec2(width * 0.75, height * 0.375), color);
    middle.vertices.emplace_back(Vec2(width * 0.25, height * 0.625), color);
    middle.vertices.emplace_back(Vec2(width * 0.75, height * 0.625), color);

    _triangle_strips.emplace_back(std::move(top));
    _triangle_strips.emplace_back(std::move(left));
    _triangle_strips.emplace_back(std::move(right));
    _triangle_strips.emplace_back(std::move(bottom));
    _triangle_strips.emplace_back(std::move(middle));

    // TODO: Textures
    for (auto& strip : _triangle_strips) {
        _polygons.emplace_back(
            std::make_shared<Polygon>(std::move(strip.create_polygons(1)[0])));
    }
}

void Walls::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto& strip : _triangle_strips)
        strip.draw(target, states);
}

}  // namespace VVipers