#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/Level.hpp>
#include <vvipers/RectBody.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

Level::Level(std::string name) : m_name(name) { constructLevel(); }

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
    RectBody* body;

    m_rects.push_back(body = new RectBody(Vec2(0, 0), Vec2(800, 5)));
    body->rectangleShape.setFillColor(sf::Color::Red);
    body->updateBodyPart("TopWall");

    m_rects.push_back(body = new RectBody(Vec2(795, 5), Vec2(5, 590)));
    body->rectangleShape.setFillColor(sf::Color::Blue);
    body->updateBodyPart("RightWall");

    m_rects.push_back(body = new RectBody(Vec2(0, 595), Vec2(800, 5)));
    body->rectangleShape.setFillColor(sf::Color::Yellow);
    body->updateBodyPart("BottomWall");

    m_rects.push_back(body = new RectBody(Vec2(0, 5), Vec2(5, 590)));
    body->rectangleShape.setFillColor(sf::Color::Green);
    body->updateBodyPart("LeftWall");

    m_rects.push_back(body = new RectBody(Vec2(300, 250), Vec2(200, 100)));
    body->rectangleShape.setFillColor(sf::Color::Magenta);
    body->updateBodyPart("MiddleWall");
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto rect : m_rects)
        target.draw(*rect, states);
}

}  // namespace VVipers