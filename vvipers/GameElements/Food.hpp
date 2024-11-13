#ifndef VVIPERS_GAMEELEMENTS_FOOD_HPP
#define VVIPERS_GAMEELEMENTS_FOOD_HPP

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <memory>
#include <vector>
#include <vvipers/GameElements/GameObject.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/Collisions/CollidingSegment.hpp"

namespace VVipers {

class Food : public GameObject, public sf::CircleShape, public CollidingBody, public Observable {
  public:
    Food(Vec2 position, double radius, Time bonusExpire, sf::Color color);
    std::vector<std::shared_ptr<const CollidingSegment>> colliding_segments() const override;
    sf::Color color() const;
    bool is_bonus_eligible() const;
    double score_value() const;
    void update(Time elapsedTime);

    static const double nominalFoodRadius;

  private:
    void decay(Time elapsedTime);

    Time _age;
    Time _bonus_expire;
    std::shared_ptr<CollidingSegment> _colliding_segment;
    double _color_hue;
    double _color_saturation;
    double _color_lightness;
    const double _original_radius;
    Time _start_of_decay;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEELEMENTS_FOOD_HPP
