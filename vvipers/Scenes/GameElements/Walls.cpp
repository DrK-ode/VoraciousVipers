#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/Scenes/GameElements/Walls.hpp>
#include <vvipers/Utilities/VVMath.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

Walls::Walls(Vec2 levelSize) : ColliderSegmented(true), m_levelSize(levelSize) {
    constructLevel();
}

void Walls::constructLevel() {
    const double width = m_levelSize.x;
    const double height = m_levelSize.y;
    const double edge = 5;
    // TODO: Textures

    m_rects.push_back({Vec2(width, edge)});
    m_rects.back().setPosition(0, 0);

    m_rects.push_back({Vec2(edge, height - 2 * edge)});
    m_rects.back().setPosition(width - edge, edge);

    m_rects.push_back({Vec2(width, edge)});
    m_rects.back().setPosition(0, height - edge);

    m_rects.push_back({Vec2(edge, height - 2 * edge)});
    m_rects.back().setPosition(0, edge);

    m_rects.push_back({Vec2(width / 2, height / 4)});
    m_rects.back().setPosition(width / 4, height * 3 / 8);
    m_rects.back().setFillColor(sf::Color(0x80, 0x80, 0x80, 255));
}

void Walls::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto rect : m_rects)
        target.draw(rect, states);
}

}  // namespace VVipers