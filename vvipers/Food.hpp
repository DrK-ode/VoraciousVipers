#ifndef VVIPERS_FOOD_HPP
#define VVIPERS_FOOD_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <vvipers/Collidable.hpp>
#include <vvipers/ConvexBody.hpp>
#include <vvipers/GameObject.hpp>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class Food : public GameObject, public sf::Drawable, public Collidable {
  public:
    Food(Vec2 position, double size) {
        m_body = ConvexBody::createCircle(position, size, 6, true);
        m_body->convexShape.setFillColor(sf::Color::Cyan);
    };
    ~Food() { delete m_body; }
    std::vector<const CollisionBody*> collisionBodies() const override {
        return std::vector<const CollisionBody*>(1, m_body);
    }
    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override {
        target.draw(*m_body, states);
    }

  private:
    ConvexBody* m_body;
};

}  // namespace VVipers

#endif  // VVIPERS_FOOD_HPP
