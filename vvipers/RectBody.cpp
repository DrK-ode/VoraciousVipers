#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <vvipers/RectBody.hpp>

namespace VVipers {

RectBody::RectBody( CBID_type id, Vec2 topLeft, Vec2 size, bool active)
    : CollisionBody( id ), rectangleShape(size), m_bodypart(nullptr) {
    rectangleShape.setPosition(topLeft);
    updateBodyPart(active);
}

const std::vector<const Bodypart*> RectBody::bodyparts() const {
    return std::vector<const Bodypart*>(1, m_bodypart);
}

void RectBody::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(rectangleShape, states);
}

void RectBody::updateBodyPart(bool active) {
    auto transform = rectangleShape.getTransform();
    std::vector<Vec2> nodes;
    // Save the nodes in an order corresponding to TriangleStrip
    nodes.push_back(transform.transformPoint(rectangleShape.getPoint(0)));
    nodes.push_back(transform.transformPoint(rectangleShape.getPoint(1)));
    nodes.push_back(transform.transformPoint(rectangleShape.getPoint(3)));
    nodes.push_back(transform.transformPoint(rectangleShape.getPoint(2)));
    delete m_bodypart;
    m_bodypart = new Bodypart(nodes, CBID, active, true,
                              sf::PrimitiveType::TriangleStrip);
}

}  // namespace VVipers