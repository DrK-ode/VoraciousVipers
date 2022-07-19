#ifndef VVIPERS_BODYPART_HPP
#define VVIPERS_BODYPART_HPP

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers {

/** Objects of this type cannot change past creation since the
 * normal axes are calculated at birth. **/
class Bodypart {
  public:
    Bodypart(const std::vector<Vec2>& nodes, const std::string& id,
             bool active = false, bool symmetric = false,
             sf::PrimitiveType vertexOrder = sf::PrimitiveType::TriangleStrip);

    static bool collision(const Bodypart*, const Bodypart*);

    bool active() const { return m_isActive; }
    const std::vector<Vec2> axes() const { return m_axes; }
    bool inside( const Vec2& point ) const;
    std::vector<Vec2> nodes() const { return m_nodes; }
    size_t numberOfNodes() const { return m_nodes.size(); }
    size_t numberOfAxes() const { return m_axes.size(); }
    sf::Rect<double> rectangularBounds() const {return m_boundingRect;}
    bool symmetric() const { return m_isSymmetric; }

    const std::string BPID;

  private:
    void update(sf::PrimitiveType vertexOrder);
    void updateAxesTriangleFan();
    void updateAxesTriangleStrip();

    bool m_isActive;
    bool m_isSymmetric;
    std::vector<Vec2> m_nodes;
    std::vector<Vec2> m_axes;
    sf::Rect<double> m_boundingRect;
};

}  // namespace VVipers

#endif  // VVIPERS_BODYPART_HPP
