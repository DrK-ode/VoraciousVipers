#ifndef VVIPERS_FOOD_HPP
#define VVIPERS_FOOD_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <vector>
#include <vvipers/Scenes/GameElements/GameObject.hpp>
#include <vvipers/Scenes/GameElements/Observer.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Scenes/Collision2/Shape.hpp>

namespace VVipers {

class Food : public GameObject,
             public CircleShape,
             public Observable {
  public:
    Food(Vec2 position, double radius)
        : CircleShape(radius, 7),
          m_startOfDecay(false) {
        setFillColor(sf::Color::Cyan);
        setPosition(position);
    }
    void update(Time elapsedTime);

    static const double nominalFoodRadius;

  private:
    void decay(Time elapsedTime);
    void stateChanged(ObjectState from, ObjectState into) override;

    bool m_startOfDecay;
};

}  // namespace VVipers

#endif  // VVIPERS_FOOD_HPP
