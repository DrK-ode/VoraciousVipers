#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/ConvexBody.hpp>
#include <vvipers/VVMath.hpp>
#include <vvipers/Walls.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

Walls::Walls(Vec2 levelSize) : m_levelSize(levelSize) { constructLevel(); }

Walls::~Walls() {
    for (auto body : m_rects)
        delete body;
}

std::vector<const CollisionBody*> Walls::collisionBodies() const {
    std::vector<const CollisionBody*> bodies;
    for (auto rect : m_rects)
        bodies.push_back(static_cast<const CollisionBody*>(rect));
    return bodies;
}

void Walls::constructLevel() {
    const double width = m_levelSize.x;
    const double height = m_levelSize.y;
    const double edge = 5;
    // TODO: Textures
    ConvexBody* body;

    m_rects.push_back(
        body = ConvexBody::createRectangle(Vec2(0, 0), Vec2(width, edge)));
    m_rects.push_back(
        body = ConvexBody::createRectangle(Vec2(width - edge, edge),
                                           Vec2(edge, height - 2 * edge)));
    m_rects.push_back(body = ConvexBody::createRectangle(Vec2(0, height - edge),
                                                         Vec2(width, edge)));
    m_rects.push_back(body = ConvexBody::createRectangle(
                          Vec2(0, edge), Vec2(edge, height - 2 * edge)));
    m_rects.push_back(
        body = ConvexBody::createRectangle(Vec2(width / 4, height * 3 / 8),
                                           Vec2(width / 2, height / 4)));

    body->convexShape.setFillColor(sf::Color(0x80, 0x80, 0x80, 255));
}

void Walls::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto rect : m_rects)
        target.draw(*rect, states);
}

}  // namespace VVipers