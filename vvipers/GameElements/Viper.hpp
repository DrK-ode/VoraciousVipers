#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <memory>
#include <vector>
#include <vvipers/Collisions/CollidingBody.hpp>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/GameElements/Food.hpp>
#include <vvipers/GameElements/GameEvent.hpp>
#include <vvipers/GameElements/GameObject.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/GameElements/Track.hpp>
#include <vvipers/GameElements/ViperConfiguration.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/TriangleStripArray.hpp>

namespace VVipers {
/**
 * A physical model of a Viper (position, speed etc.).
 * Handles all aspects of the Viper that is not related to the graphical
 * representation.
 */
class Viper : public GameObject,
              public sf::Drawable,
              public CollidingBody,
              public Observable {
  public:
    Viper(std::shared_ptr<const ViperConfiguration>, const Vec2& tail_position,
          double angle, double number_of_body_segments);
    size_t number_of_segments() const override { return _polygons.size(); }
    std::shared_ptr<const Shape> segment_shape(size_t index) const override {
        return _polygons[index];
    }
    /** Adds time the Viper should spend growing and where along the viper that
     * growth is. **/
    void add_growth(Time howMuch, Time when, sf::Color color);
    /** @return current direction of the head. **/
    double angle() const { return _angle; }
    /** Sets the direction of the head and keeps the angle stored within ±180
     * degrees. **/
    void set_angle(double a) { _angle = mod_180_deg(a); }
    /** @returns the amount of boost the Viper is currently receiving **/
    double boost_amount() const { return _boost_increase; }
    /** @returns the current stored boost duration **/
    Time boost_charge() const { return _boost_charge; }
    /** Adds boost charge but won't exceed maximum **/
    void add_boost_charge(Time charge);
    /** @returns the maximum stored boost duration **/
    Time boost_max() const;
    sf::Color primary_color() const { return _primaryColor; }
    sf::Color secondary_color() const { return _secondaryColor; }
    void set_colors(sf::Color primary_color, sf::Color secondary_color) {
        _primaryColor = primary_color;
        _secondaryColor = secondary_color;
    }
    /** Consumes food that will add both growth and some boost charge **/
    void eat(const Food& food);
    /** Changes state to Dying and will eventually become Dead **/
    void die(const Time& elapsedTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    /** @returns The track the viper follows. **/
    const TemporalTrack& temporal_track() const { return *_track; }
    /** @returns the spatial length of the Viper. **/
    double length() const;
    void set_speed(double s) { _speed = s; }
    double speed() const { return _speed; }
    void set_nominal_speed(double s) { _nominalSpeed = s; }
    double nominal_speed() const { return _nominalSpeed; }
    /** Sets how fast the angle changes as well as the boost factor **/
    void steer(double angularSpeed, double boost) {
        _angular_speed = angularSpeed;
        _boost_increase = boost;
    }
    // The time it takes for the tail to reach the current position of the head.
    Time temporal_length() const { return _temporal_length; }
    /** @returns the minimum turning radius **/
    double max_angular_speed() const;
    /** Updates state of the Viper. Should normally be called by the onNotify
     * member function. **/
    void update(Time elapsedTime);
    Vec2 velocity() const { return Vec2(_speed, 0).rotate(_angle); }
    const ViperConfiguration& viper_configuration() const {
        return *_viper_configuration.get();
    }

  private:
    std::shared_ptr<const ViperConfiguration> _viper_configuration;

    enum class ViperPart { Head, Body, Tail };
    void create_next_head_temporal_track_point(Time elapsedTime);
    void clean_up_trailing_temporal_track_points();
    void clean_up_dinner_times();
    void grow(const Time& elapsedTime);

    void update_vertices_and_polygons();
    void create_vertex_vectors_and_polygons_for_body_part(
        ViperPart, const Time& timeFront, const Time& temporalLength);
    sf::Color calculate_vertex_color(Time time);

    void update_motion(const Time& elapsedTime);
    void update_speed(const Time& elapsedTime);
    void update_angle(const Time& elapsedTime);
    void update_boost_charge(Time charge);

    double _angle;          // degrees, clockwise since y-axis is downwards
    double _angular_speed;  // degrees/s
    double _nominalSpeed;   // px/s
    double _speed;          // px/s
    // double m_targetSpeed;   // px/s
    double _boost_increase;  // Boost speed = (1 + m_boost) * nominal speed
    Time _boost_charge;      // fraction [0., 1.]
    Time _boost_recharge_cooldown;  // Countdown from viperBoostChargeCooldown
    Time _temporal_length;          // s
    Time _growth;                   // s
    std::unique_ptr<TemporalTrack> _track;
    sf::Color _primaryColor;
    sf::Color _secondaryColor;
    struct Dinner {
        Time amount;
        sf::Color color;
    };
    std::map<Time, Dinner> _dinner_times;

    TriangleStripArray _triangle_strip_head;
    TriangleStripArray _triangle_strip_body;
    TriangleStripArray _triangle_strip_tail;

    std::vector<std::shared_ptr<Polygon>> _polygons;
};

}  // namespace VVipers
