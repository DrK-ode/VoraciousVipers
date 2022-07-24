#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/Level.hpp>
#include <vvipers/RectBody.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

Level::Level(const std::string& name, Vec2 levelSize) : m_name(name) { constructLevel(levelSize); }

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

void Level::constructLevel(Vec2 levelSize) {
    const double width = levelSize.x;
    const double height = levelSize.y;
    const double edge = 5;
    //TODO: Provide starting points
    RectBody* body;

    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(0, 0), Vec2(width, edge)));
    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(width - edge, edge), Vec2(edge, height - 2*edge)));
    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(0, height - edge), Vec2(width, edge)));
    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(0, edge), Vec2(edge, height-2*edge)));
    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(width/4, height*3/8), Vec2(width/2, height/4)));
    body->rectangleShape.setFillColor(sf::Color(0x80,0x80,0x80,255));
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto rect : m_rects)
        target.draw(*rect, states);
}

}  // namespace VVipers