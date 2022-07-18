#include <vvipers/Level.hpp>
#include <vvipers/debug.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace VVipers {

void Level::constructLevel() {
    m_collisionBody.resize(16);

    m_collisionBody.vertex(0, sf::Vertex(Vec2(0, 0), sf::Color::Red));
    m_collisionBody.vertex(1, sf::Vertex(Vec2(800, 0), sf::Color::Red));
    m_collisionBody.vertex(2, sf::Vertex(Vec2(800, 5), sf::Color::Red));
    m_collisionBody.vertex(3, sf::Vertex(Vec2(0, 5), sf::Color::Red));

    m_collisionBody.vertex(4, sf::Vertex(Vec2(0, 5), sf::Color::Red));
    m_collisionBody.vertex(5, sf::Vertex(Vec2(5, 5), sf::Color::Red));
    m_collisionBody.vertex(6, sf::Vertex(Vec2(5, 595), sf::Color::Red));
    m_collisionBody.vertex(7, sf::Vertex(Vec2(0, 595), sf::Color::Red));

    m_collisionBody.vertex(8, sf::Vertex(Vec2(795, 5), sf::Color::Red));
    m_collisionBody.vertex(9, sf::Vertex(Vec2(800, 5), sf::Color::Red));
    m_collisionBody.vertex(10, sf::Vertex(Vec2(800, 595), sf::Color::Red));
    m_collisionBody.vertex(11, sf::Vertex(Vec2(795, 595), sf::Color::Red));

    m_collisionBody.vertex(12, sf::Vertex(Vec2(0, 595), sf::Color::Red));
    m_collisionBody.vertex(13, sf::Vertex(Vec2(800, 595), sf::Color::Red));
    m_collisionBody.vertex(14, sf::Vertex(Vec2(800, 600), sf::Color::Red));
    m_collisionBody.vertex(15, sf::Vertex(Vec2(0, 600), sf::Color::Red));

    m_collisionBody.assignBodyParts(0, m_collisionBody.size(), "Wall", 4, 0,
                                    false, true);
                                    tagInfo( m_collisionBody.bodyParts().size() );
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_collisionBody, states);
}

}  // namespace VVipers