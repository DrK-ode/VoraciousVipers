#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/Level.hpp>
#include <vvipers/debug.hpp>
#include <vvipers/RectBody.hpp>

namespace VVipers {

Level::Level(std::string name) : m_name(name) { constructLevel(); }

Level::~Level() {
    for (auto body : m_collisionBodies)
        delete body;
}

std::vector<const CollisionBody*> Level::collisionBodies() const {
    return m_collisionBodies;
}

void Level::constructLevel() {
    RectBody* body;

    m_collisionBodies.push_back(body = new RectBody(Vec2(0, 0), Vec2(800, 5)));
    body->setColor(sf::Color::Red);
    body->setBodyPart("TopWall");

    m_collisionBodies.push_back(body =
                                    new RectBody(Vec2(795, 5), Vec2(5, 590)));
    body->setColor(sf::Color::Blue);
    body->setBodyPart("RightWall");

    m_collisionBodies.push_back(body =
                                    new RectBody(Vec2(0, 595), Vec2(800, 5)));
    body->setColor(sf::Color::Yellow);
    body->setBodyPart("BottomWall");

    m_collisionBodies.push_back(body = new RectBody(Vec2(0, 5), Vec2(5, 590)));
    body->setColor(sf::Color::Green);
    body->setBodyPart("LeftWall");

    m_collisionBodies.push_back(body = new RectBody(Vec2(300, 250), Vec2(200, 100)));
    body->setColor(sf::Color::Red, sf::Color::Green, sf::Color::Magenta, sf::Color::Blue);
    body->setBodyPart("MiddleWall");
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto body : m_collisionBodies)
        target.draw(*body, states);
}

}  // namespace VVipers