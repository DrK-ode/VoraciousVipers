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
#include <vvipers/ViperConfig.hpp>

namespace VVipers {
/**
 * A physical model of a Viper (position, speed etc.).
 * Handles all aspects of the Viper that is not related to the graphical
 * representation.
 */
class Viper : public GameObject,
              public Collidable,
              public sf::Drawable,
              public Observer,
              public Observable {
  public:
    Viper();

    enum ViperPart : PartID_t {
        ViperHead = 1 << 0,
        ViperBody = 1 << 1,
        ViperTail = 1 << 2,
        ViperSensitivePart = 1 << 16
    };
    typedef uint64_t ViperState_t;
    enum ViperState : ViperState_t {
        ViperAlive = 1 << 0,
        ViperDead = 1 << 1,
        ViperStasis = 1 << 2,
        ViperDying = ViperAlive | ViperDead
    };

  public:
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
    Time boostMax() const { return ViperConfig::properties().boostMaxCharge; }
    /** Collidable override. @returns a list of all owned CollidableBody which
     * for a Viper is the head, body and tail. **/
    std::vector<const CollisionBody*> collisionBodies() const override {
        return std::vector<const CollisionBody*>(
            {&m_headBody, &m_bodyBody, &m_tailBody});
    }
    void die(const Time& elapsedTime);
    /** Drawable override. Draws all parts of the viper to the target **/
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    /** @returns The track the viper follows. **/
    const Track& getTrack() const { return m_track; }
    /** Adds time the Viper should spend growing. **/
    void growth(const Time& g);
    /** @returns The first point on the track the Viper is following. **/
    const TrackPoint* head() const { return m_headPoint; }
    /** @returns Normal (spatial) length of the Viper. **/
    double length() const;
    /** Observer override. Listens to update events **/
    virtual void onNotify(const GameEvent* event) override;
    /** Initiliazes the position and direction of the Viper given the specified
     * length. **/
    void setup(const Vec2& from, double angle, double numberOfBodySegments);
    /** Sets the speed. **/
    void speed(double s) { m_speed = s; }
    /** @returns current speed. **/
    double speed() const { return m_speed; }
    ViperState_t state() const { return m_state; }
    void state(ViperState state) { m_state = state; }
    void steer(double angularSpeed, double boost) {
        m_angularSpeed = angularSpeed;
        m_boostInc = boost;
    }
    /** @returns The temporal length of the Viper.
     * It is the time it takes for the tail to reach the current position of the
     * head. **/
    Time temporalLength() const { return m_temporalLength; }
    /** @returns the minimum turning radius **/
    double turningRadius() const {
        return ViperConfig::properties().nominalSpeed /
               ViperConfig::properties().nominalSegmentWidth;
    }
    /** Updates state of the Viper. Should normally be called by the onNotify
     * member function. **/
    void update(const Time& elapsedTime);

  private:
    TrackPoint* createNextHeadTrackPoint(Time elapsedTime);
    void cleanUpTrailingTrackPoints();
    void grow(const Time& elapsedTime);
    void loadTextures();

    void updateBodies();
    void updateBody(CollisionVertices&, Time timeFront,
                    const Time& temporalLength);
    void updateMotion(const Time& elapsedTime);
    void updateSpeed(const Time& elapsedTime);
    void updateAngle(const Time& elapsedTime);

    ViperState_t m_state;
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
    sf::Color m_mainColor;
    sf::Texture m_headTexture;
    sf::Texture m_bodyTexture;
    sf::Texture m_tailTexture;
};

}  // namespace VVipers
#endif  // VVIPERS_VIPER_HPP
