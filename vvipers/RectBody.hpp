#ifndef VVIPERS_RECTBODY_HPP
#define VVIPERS_RECTBODY_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>
#include <vvipers/Bodypart.hpp>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class Bodypart;

class RectBody : public CollisionBody, public sf::Drawable {
  public:
    RectBody(Vec2 topLeft, Vec2 size, const std::string& label = "",
             bool active = false);

    virtual const std::vector<const Bodypart*> bodyParts() const override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    /** updateBodyPart must be called after changing the geometry of the
     * rectangle but also to change the label or active state. These four
     * versions of the update function makes sure that no unspecified properties
     * are changed. **/
    void updateBodyPart() {
        updateBodyPart(m_bodypart->label(), m_bodypart->active());
    }
    void updateBodyPart(const std::string& label) {
        updateBodyPart(label, m_bodypart->active());
    }
    void updateBodyPart(bool active) {
        updateBodyPart(m_bodypart->label(), active);
    }
    void updateBodyPart(const std::string& label, bool active);

    sf::RectangleShape rectangleShape;

  private:
    const Bodypart* m_bodypart;
};

}  // namespace VVipers

#endif  // VVIPERS_RECTBODY_HPP
