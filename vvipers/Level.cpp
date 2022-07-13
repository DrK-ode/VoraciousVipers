#include <vvipers/Level.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

Level::~Level() {
    for (auto cr : m_collidableParts)
        delete cr;
    for (auto s : m_shapes)
        delete s;
}

void Level::constructLevel() {
    m_collidableParts.push_back(
        new CollidableRect(Vec2(0, 0), Vec2(795, 5), "EdgeTop"));
    m_collidableParts.push_back(
        new CollidableRect(Vec2(795, 0), Vec2(5, 595), "EdgeRight"));
    m_collidableParts.push_back(
        new CollidableRect(Vec2(5, 595), Vec2(795, 5), "EdgeBottom"));
    m_collidableParts.push_back(
        new CollidableRect(Vec2(0, 5), Vec2(5, 595), "EdgeLeft"));

    for (auto cpi : m_collidableParts) {
        CollidableRect* cr = static_cast<CollidableRect*>(cpi);
        sf::Shape* s = new sf::RectangleShape(cr->size());
        s->setPosition(cr->position());
        s->setFillColor(sf::Color::Red);
        m_shapes.push_back(s);
    }
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto shape : m_shapes)
        target.draw(*shape, states);
}

}  // namespace VVipers