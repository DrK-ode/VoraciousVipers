#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/Level.hpp>
#include <vvipers/RectBody.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

Level::Level(const std::string& name) : m_name(name) { constructLevel(); }

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
    //TODO: Be aware of the window size
    //TODO: Provide starting points
    RectBody* body;

    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(0, 0), Vec2(800, 5)));
    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(795, 5), Vec2(5, 590)));
    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(0, 595), Vec2(800, 5)));
    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(0, 5), Vec2(5, 590)));
    m_rects.push_back(body = new RectBody(LevelWall,
                                          Vec2(300, 250), Vec2(200, 100)));
    body->rectangleShape.setFillColor(sf::Color::Magenta);
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto rect : m_rects)
        target.draw(*rect, states);
}

}  // namespace VVipers