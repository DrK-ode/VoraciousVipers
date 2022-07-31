#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <vvipers/Scenes/Collision/ConvexBody.hpp>

namespace VVipers {

ConvexBody::ConvexBody(Vec2 position, const std::vector<Vec2>& nodes,
                       bool active, bool symmetric)
    : m_bodypart(nullptr) {
    convexShape.setPointCount(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i)
        convexShape.setPoint(i, sf::Vector2f(nodes[i]));

    convexShape.setPosition(position);
    updateBodyPart(active, symmetric);
}

ConvexBody* ConvexBody::createCircle(Vec2 position, double r, size_t n,
                                     bool active) {
    std::vector<Vec2> nodes;
    nodes.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        double angle = twopi * i / n;
        nodes.push_back({r * std::cos(angle), r * std::sin(angle)});
    }

    // In this case symmetric only applies if for any edge is an opposite
    // parallel edge
    bool symmetric = n % 2 == 0;
    return new ConvexBody(position, nodes, active, symmetric);
}

ConvexBody* ConvexBody::createRectangle(Vec2 topLeft, Vec2 size, bool active) {
    std::vector<Vec2> nodes;
    nodes.push_back({0, 0});
    nodes.push_back({size.x, 0});
    nodes.push_back(size);
    nodes.push_back({0, size.y});

    return new ConvexBody(topLeft, nodes, active, true);
}

const std::vector<const Bodypart*> ConvexBody::bodyparts() const {
    return std::vector<const Bodypart*>(1, m_bodypart);
}

void ConvexBody::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(convexShape, states);
}

void ConvexBody::updateBodyPart(bool active, bool symmetric) {
    auto transform = convexShape.getTransform();
    std::vector<Vec2> nodes;
    // Save the nodes in an order corresponding to TriangleFan
    for (size_t i = 0; i < convexShape.getPointCount(); ++i)
        nodes.push_back(transform.transformPoint(convexShape.getPoint(i)));
    delete m_bodypart;
    m_bodypart =
        new Bodypart(nodes, active, false, sf::PrimitiveType::TriangleFan);
}

}  // namespace VVipers