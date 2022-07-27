#ifndef VVIPERS_FOOD_HPP
#define VVIPERS_FOOD_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <vvipers/Collidable.hpp>
#include <vvipers/ConvexBody.hpp>
#include <vvipers/GameObject.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/Time.hpp>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class Food : public GameObject,
             public sf::Drawable,
             public MonoBodyCollidable,
             public Observable {
  public:
    Food(Vec2 position, double size)
        : MonoBodyCollidable(
              ConvexBody::createCircle(position, size / 2, 7, true)),
          m_startOfDecay(false), m_size(size) {
        m_body = (ConvexBody*)body();
        m_body->convexShape.setFillColor(sf::Color::Cyan);
    }
    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override {
        target.draw(*m_body, states);
    }
    void update(Time elapsedTime);
    Vec2 getPosition() const {return m_body->convexShape.getPosition();}
    double getSize() const {return m_size;}

    static const double nominalFoodSize;

  private:
    void decay(Time elapsedTime);
    void stateChanged(ObjectState from, ObjectState into) override;

    bool m_startOfDecay;
    double m_size;
    ConvexBody* m_body;
};

}  // namespace VVipers

#endif  // VVIPERS_FOOD_HPP
