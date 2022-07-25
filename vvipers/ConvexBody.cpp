#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <vvipers/ConvexBody.hpp>

namespace VVipers {

ConvexBody::ConvexBody(Vec2 position, const std::vector<Vec2>& nodes,
                       bool active)
    : m_bodypart(nullptr) {
    convexShape.setPointCount(nodes.size());
    for (int i = 0; i < nodes.size(); ++i) {
        convexShape.setPoint(i, sf::Vector2f(nodes[i]));
        m_relativeCenter += nodes[i];
    }
    m_relativeCenter / nodes.size();

    convexShape.setPosition(position);
    updateBodyPart(active);
}

ConvexBody* ConvexBody::createCircle(Vec2 position, double r, size_t n,
                                     bool active) {
    std::vector<Vec2> nodes;
    nodes.reserve(n);
    for (int i = 0; i < n; + i) {
        double angle = twopi * i / n;
        nodes.push_back({r * std::cos(angle), r * std::sin(angle)});
    }

    return new ConvexBody(position, nodes, active);
}

ConvexBody* ConvexBody::createRectangle(Vec2 topLeft, Vec2 size, bool active) {
    std::vector<Vec2> nodes;
    nodes.push_back({0, 0});
    nodes.push_back({size.x, 0});
    nodes.push_back(size);
    nodes.push_back({0, size.y});

    return new ConvexBody(topLeft, nodes, active);
}

const std::vector<const Bodypart*> ConvexBody::bodyparts() const {
    return std::vector<const Bodypart*>(1, m_bodypart);
}

void ConvexBody::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(convexShape, states);
}

void ConvexBody::updateBodyPart(bool active) {
    auto transform = convexShape.getTransform();
    std::vector<Vec2> nodes;

    // Save the nodes in an order corresponding to TriangleFan
    nodes.push_back(transform.transformPoint(m_relativeCenter));
    for (int i = 1; i < convexShape.getPointCount(); ++i)
        nodes.push_back(convexShape.getPoint(i));
    delete m_bodypart;
    m_bodypart =
        new Bodypart(nodes, active, true, sf::PrimitiveType::TriangleFan);
}

}  // namespace VVipers