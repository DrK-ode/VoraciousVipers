#ifndef VVIPERS_CONVEXBODY_HPP
#define VVIPERS_CONVEXBODY_HPP

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <vvipers/Bodypart.hpp>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/Vec2.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

class ConvexBody : public CollisionBody, public sf::Drawable {
  public:
    ConvexBody(PartID_t id, Vec2 position, const std::vector<Vec2>& nodes,
               bool active = false);
    static ConvexBody* createRectangle(PartID_t id, Vec2 topLeft, Vec2 size, bool active = false);

    virtual const std::vector<const Bodypart*> bodyparts() const override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    /** updateBodyPart must be called after changing the geometry of the
     * rectangle but also to change the active state. **/
    void updateBodyPart() { updateBodyPart(m_bodypart->active()); }
    void updateBodyPart(bool active);
    sf::Rect<double> rectangularBounds() const override {
        return sf::Rect<double>(convexShape.getGlobalBounds());
    }

    sf::ConvexShape convexShape;

  private:
    Vec2 m_relativeCenter;
    const Bodypart* m_bodypart;
};

}  // namespace VVipers

#endif // VVIPERS_CONVEXBODY_HPP
