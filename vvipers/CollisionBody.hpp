#ifndef VVIPERS_COLLISIONBODY_HPP
#define VVIPERS_COLLISIONBODY_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>
#include <vvipers/Vec2.hpp>
#include <vvipers/BodyPart.hpp>

namespace VVipers {

/** No parts of a CollisionBody will ever be checked for collisions between each
 * other **/
class CollisionBody : public sf::Drawable {
  public:
    CollisionBody(
        sf::PrimitiveType vertexType = sf::PrimitiveType::TriangleStrip)
        : m_hasActivePart(false), m_texture(nullptr), m_vertices(vertexType) {
        for (auto& part : m_bodyParts)
            if (part.active()) {
                m_hasActivePart = true;
                break;
            }
    }
    bool isActive() const { return m_hasActivePart; }
    static std::vector<std::pair<const BodyPart*, const BodyPart*> > collision(
        const CollisionBody&, const CollisionBody&);
    void clear() {
        m_vertices.clear();
        m_bodyParts.clear();
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void resize(size_t s) { m_vertices.resize(s); }
    size_t size() const { return m_vertices.getVertexCount(); }
    void setTexture(const sf::Texture* texture) { m_texture = texture; }
    sf::Vertex vertex(int i) const { return m_vertices[i]; }
    void vertex(int i, const sf::Vertex& v) { m_vertices[i] = v; }
    void appendVertex(const sf::Vertex& v) { m_vertices.append(v); }
    void vertexType(sf::PrimitiveType vertexType) {
        m_vertices.setPrimitiveType(vertexType);
    }

    void assignBodyParts(size_t beginIndex, size_t length, std::string label,
                         size_t nodesPerBodyPart, int sharedNodes = 0,
                         bool active = false, bool symmetric = false);
    const std::vector<BodyPart>& bodyParts() const { return m_bodyParts; }

  private:
    bool m_hasActivePart;
    std::vector<BodyPart> m_bodyParts;
    sf::VertexArray m_vertices;
    sf::Color m_color;
    const sf::Texture* m_texture;
};

}  // namespace VVipers

#endif // VVIPERS_COLLISIONBODY_HPP
