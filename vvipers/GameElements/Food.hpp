#ifndef VVIPERS_FOOD_HPP
#define VVIPERS_FOOD_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>
#include <vvipers/Collision/Shape.hpp>
#include <vvipers/GameElements/GameObject.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

class Food : public GameObject, public CircleShape, public Observable {
  public:
    Food(Vec2 position, double radius, Time bonusExpire, sf::Color color);
    bool isBonusEligible() const;
    double getScoreValue() const;
    sf::Color getColor() const;
    void update(Time elapsedTime);

    static const double nominalFoodRadius;

  private:
    void decay(Time elapsedTime);

    Time m_startOfDecay;
    Time m_age;
    Time m_bonusExpire;
    const double m_originalRadius;
    double m_colorH;
    double m_colorS;
    double m_colorL;
};

}  // namespace VVipers

#endif  // VVIPERS_FOOD_HPP
