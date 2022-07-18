#ifndef VVIPERS_BODYPART_HPP
#define VVIPERS_BODYPART_HPP

#include <SFML/Graphics/PrimitiveType.hpp>
#include <string>
#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers {

/** Objects of this type cannot change past creation since the
 * normal axes are calculated at birth. **/
class BodyPart {
  public:
    BodyPart(const std::vector<Vec2>& nodes, const std::string& label = "",
             bool active = false, bool symmetric = false,
             sf::PrimitiveType vertexOrder = sf::PrimitiveType::TriangleStrip);

    static bool collision(const BodyPart&, const BodyPart&);

    bool active() const { return m_isActive; }
    const std::vector<Vec2> axes() const { return m_axes; }
    std::string label() const { return m_label; }
    std::vector<Vec2> nodes() const { return m_nodes; }
    size_t numberOfNodes() const { return m_nodes.size(); }
    size_t numberOfAxes() const { return m_axes.size(); }
    bool symmetric() const { return m_isSymmetric; }

  private:
    void updateAxes(sf::PrimitiveType vertexOrder);
    void updateAxesTriangleFan();
    void updateAxesTriangleStrip();

    bool m_isActive;
    bool m_isSymmetric;
    std::string m_label;
    std::vector<Vec2> m_nodes;
    std::vector<Vec2> m_axes;
};

}  // namespace VVipers

#endif  // VVIPERS_BODYPART_HPP
