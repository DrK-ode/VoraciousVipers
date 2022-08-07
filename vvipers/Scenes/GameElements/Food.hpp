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
    Food(Vec2 position, double radius, Time bonusExpire);
    bool isBonusEligible() const;
    double getScoreValue() const;
    void update(Time elapsedTime);

    static const double nominalFoodRadius;

  private:
    void decay(Time elapsedTime);

    Time m_startOfDecay;
    Time m_age;
    Time m_bonusExpire;
    const double m_originalRadius;
    const double m_hue;
};

}  // namespace VVipers

#endif  // VVIPERS_FOOD_HPP
