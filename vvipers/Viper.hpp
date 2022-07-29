#ifndef VVIPERS_VIPER_HPP
#define VVIPERS_VIPER_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <vvipers/Bodypart.hpp>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionVertices.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/GameObject.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/Time.hpp>
#include <vvipers/Track.hpp>

namespace VVipers {
/**
 * A physical model of a Viper (position, speed etc.).
 * Handles all aspects of the Viper that is not related to the graphical
 * representation.
 */
class Viper : public GameObject,
              public Collidable,
              public sf::Drawable,
              public Observable {
  public:
    Viper();
    /** Adds time the Viper should spend growing. **/
    void addGrowth(const Time& g);
    /** @return current direction of the head. **/
    double angle() const { return m_angle; }
    /** Sets the direction of the head and keeps the angle stored within ±180
     * degrees. **/
    void angle(double a) { m_angle = mod180Deg(a); }
    /** @returns the amount of boost the Viper is currently receiving **/
    double boost() const { return m_boostInc; }
    /** Sets the relative amount the speed should be increased by.
     * Typically values around 1.0. **/
    void boost(double relativeSpeedIncrease);
    /** @returns the current stored boost duration **/
    Time boostCharge() const { return m_boostCharge; }
    /** @returns the maximum stored boost duration **/
    Time boostMax() const;
    /** Collidable override. @returns a list of all owned CollidableBody which
     * for a Viper is the head, body and tail. **/
    std::vector<const CollisionBody*> collisionBodies() const override {
        return std::vector<const CollisionBody*>(
            {&m_headBody, &m_bodyBody, &m_tailBody});
    }
    sf::Color color() const { return m_color; }
    void color(sf::Color c) { m_color = c; }
    /** Changes state to Dying and will evntually become dead **/
    void die(const Time& elapsedTime);
    /** Drawable override. Draws all parts of the viper to the target **/
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    /** @returns The track the viper follows. **/
    const Track& getTrack() const { return m_track; }
    /** @returns The first point on the track the Viper is following. **/
    const TrackPoint* head() const { return m_headPoint; }
    /** @returns true if the Bodypart is one of the Vipers vulnerable parts **/
    bool isSensitive(const Bodypart*) const;
    /** @returns Normal (spatial) length of the Viper. **/
    double length() const;
    /** Initiliazes the position and direction of the Viper given the specified
     * length. **/
    void setup(const Vec2& from, double angle, double numberOfBodySegments);
    /** Sets the speed. **/
    void speed(double s) { m_speed = s; }
    /** @returns current speed. **/
    double speed() const { return m_speed; }
    void steer(double angularSpeed, double boost) {
        m_angularSpeed = angularSpeed;
        m_boostInc = boost;
    }
    /** @returns The temporal length of the Viper.
     * It is the time it takes for the tail to reach the current position of the
     * head. **/
    Time temporalLength() const { return m_temporalLength; }
    /** @returns the minimum turning radius **/
    double turningRadius() const;
    /** Updates state of the Viper. Should normally be called by the onNotify
     * member function. **/
    void update(Time elapsedTime);

  private:
    struct ViperConfiguration;
    static ViperConfiguration viperCfg;

    enum class ViperPart { Head, Body, Tail };
    TrackPoint* createNextHeadTrackPoint(Time elapsedTime);
    void cleanUpTrailingTrackPoints();
    void grow(const Time& elapsedTime);

    void updateBodies();
    void updateBody(ViperPart, Time timeFront, const Time& temporalLength);
    void updateMotion(const Time& elapsedTime);
    void updateSpeed(const Time& elapsedTime);
    void updateAngle(const Time& elapsedTime);
    void updateBoostCharge(Time charge);

    double m_angle;         // degrees, clockwise since y-axis is downwards
    double m_angularSpeed;  // degrees/s
    double m_speed;         // px/s
    double m_targetSpeed;   // px/s
    double m_boostInc;      // Boost speed = (1 + m_boost) * nominal speed
    Time m_boostCharge;     // fraction [0., 1.]
    Time m_boostRechargeCooldown;  // Countdown from viperBoostChargeCooldown
    Time m_temporalLength;         // s
    Time m_growth;                 // s
    TrackPoint* m_headPoint;
    Track m_track;
    CollisionVertices m_headBody;
    CollisionVertices m_bodyBody;
    CollisionVertices m_tailBody;
    std::vector<const Bodypart*> m_sensitiveParts;
    sf::Color m_color;
};

}  // namespace VVipers
#endif  // VVIPERS_VIPER_HPP
