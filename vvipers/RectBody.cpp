#include <vvipers/RectBody.hpp>

namespace VVipers{

RectBody::RectBody(Vec2 topLeft, Vec2 size) {
    m_vertices.append(sf::Vertex(topLeft));
    m_vertices.append(sf::Vertex(topLeft + Vec2(size.x, 0)));
    m_vertices.append(sf::Vertex(topLeft + Vec2(0, size.y)));
    m_vertices.append(sf::Vertex(topLeft + size));

    setBodyPart();
}

void RectBody::setColor(sf::Color color) {
    setColor(color, color, color, color);
}

void RectBody::setColor(sf::Color c1, sf::Color c2, sf::Color c3,
                        sf::Color c4) {
    m_vertices[0].color = c1;
    m_vertices[1].color = c2;
    m_vertices[2].color = c4;
    m_vertices[3].color = c3;
}

void RectBody::setTextureCoords(Vec2 topLeft, Vec2 size) {
    m_vertices[0] = sf::Vertex(topLeft);
    m_vertices[1] = sf::Vertex(topLeft + Vec2(size.x, 0));
    m_vertices[2] = sf::Vertex(topLeft + Vec2(0, size.y));
    m_vertices[3] = sf::Vertex(topLeft + size);
}

void RectBody::setBodyPart(const std::string& label, bool active) {
    m_bodyParts.clear();
    assignBodyParts(0, 4, 4, label, 0, active, true);
}

}