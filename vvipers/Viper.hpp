#ifndef VVIPERS_VIPERPHYSICS_HPP
#define VVIPERS_VIPERPHYSICS_HPP

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
              public Observer,
              public Observable {
  public:
    Viper();

    enum ViperPart : PartID_t {
        ViperHead = 1 << 0,
        ViperBody = 1 << 1,
        ViperTail = 1 << 3,
        ViperSensitivePart = 1 << 16
    };
    typedef uint64_t ViperState_t;
    enum ViperState : ViperState_t {
        ViperAlive = 1 << 0,
        ViperDead = 1 << 1,
        ViperStasis = 1 << 2,
        ViperDying = ViperAlive | ViperDead
    };

    /** @return current direction of the head. **/
    double angle() const { return m_angle; }
    std::vector<const CollisionBody*> collisionBodies() const override {
        return std::vector<const CollisionBody*>(
            {&m_headBody, &m_bodyBody, &m_tailBody});
    }
    void die(const Time& elapsedTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    /** @return The track the viper follows. **/
    const Track& getTrack() const { return m_track; }
    /** Adds time the Viper should spend growing. **/
    void growth(const Time& g);
    /** @return The first (front) point on the track the Viper is following. **/
    const TrackPoint* head() const { return m_headPoint; }
    /** @return Normal (spatial) length of the Viper. **/
    double length() const;
    // Listen to update events
    virtual void onNotify(const GameEvent* event) override;
    /** Sets the direction of the head. **/
    void setAngle(double a) { m_angle = mod180Deg(a); }
    /** Initiliazes the position and direction of the Viper given the specified
     * length. **/
    void setup(const Vec2& from, double angle, const Time& length);
    void speed(double s) { m_speed = s; }
    /** @return current speed. **/
    double speed() const { return m_speed; }
    ViperState_t state() const { return m_state; }
    void state(ViperState state) { m_state = state; }
    // Adjust speed, angle, etc. according to the SteeringEvent
    void steer(const SteeringEvent* orders);
    /** The temporal length of the Viper.
     * It is the time it takes for the tail to reach the current position of the
     * head. **/
    Time temporalLength() const { return m_temporalLength; }
    /** Updates state of the Viper. Should be called every frame. **/
    void update(const Time& elapsedTime);

  private:
    static const Time s_headTemporalLength;
    static const Time s_bodyTemporalLength;
    static const Time s_tailTemporalLength;  // A minimum, can be larger
    TrackPoint* createNextHeadTrackPoint(Time elapsedTime);
    void cleanUpTrailingTrackPoints();
    void grow(const Time& elapsedTime);
    void loadTextures();

    void updateBodies();
    void updateBody(CollisionVertices&, const Time& timeFront,
                    const Time& temporalLength);

    ViperState_t m_state;
    static const double s_nominalSpeed;  // px/s
    double m_angle;  // degrees, clockwise since y-axis is downwards
    double m_speed;  // px/s
    double m_acc;    // px/s²
    Time m_temporalLength;
    Time m_growth;
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
#endif  // VVIPERS_VIPERPHYSICS_HPP
