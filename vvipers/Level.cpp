#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/ConvexBody.hpp>
#include <vvipers/Level.hpp>
#include <vvipers/VVMath.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

Level::Level(const std::string& name, Vec2 levelSize)
    : m_name(name), m_levelSize(levelSize) {
    constructLevel();
}

Level::~Level() {
    for (auto body : m_rects)
        delete body;
}

std::vector<const CollisionBody*> Level::collisionBodies() const {
    std::vector<const CollisionBody*> bodies;
    for (auto rect : m_rects)
        bodies.push_back(static_cast<const CollisionBody*>(rect));
    return bodies;
}

void Level::constructLevel() {
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

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto rect : m_rects)
        target.draw(*rect, states);
}

Vec2 Level::findEmptyArea(Vec2 rectSize) const {
    // If one million is not enough something is wrong or the lavel design is
    // bad
    const int maxTries = 1000000;
    for (int i = 0; i < maxTries; ++i) {
        Vec2 position(Random::getDouble(0., m_levelSize.x - rectSize.x),
                      Random::getDouble(0., m_levelSize.y - rectSize.y));
        ConvexBody* testBody =
            ConvexBody::createRectangle(position, rectSize, true);
        bool spaceFree = true;
        for (const auto cb : m_rects) {
            if (!CollisionBody::collision(testBody, cb).empty()) {
                spaceFree = false;
                break;
            }
        }
        delete testBody;
        if (spaceFree)
            return position;
    }
    throw std::runtime_error("Could not find an empty area.");
}

}  // namespace VVipers