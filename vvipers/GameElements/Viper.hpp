#ifndef VVIPERS_SCENES_GAMEELEMENTS_VIPER_HPP
#define VVIPERS_SCENES_GAMEELEMENTS_VIPER_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <memory>
#include <vector>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/Collision/Collider.hpp>
#include <vvipers/GameElements/Food.hpp>
#include <vvipers/GameElements/GameEvent.hpp>
#include <vvipers/GameElements/GameObject.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/GameElements/Track.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {
/**
 * A physical model of a Viper (position, speed etc.).
 * Handles all aspects of the Viper that is not related to the graphical
 * representation.
 */
class Viper : public GameObject,
              public sf::Drawable,
              public ColliderSegmented,
              public Observable {
  public:
    Viper(const OptionsProvider& options, const TextureProvider& textures);
    /** Adds time the Viper should spend growing and where along the viper that
     * growth is. **/
    void addGrowth(Time howMuch, Time when);
    /** @return current direction of the head. **/
    double getAngle() const { return m_angle; }
    /** Sets the direction of the head and keeps the angle stored within ±180
     * degrees. **/
    void setAngle(double a) { m_angle = mod180Deg(a); }
    /** @returns the amount of boost the Viper is currently receiving **/
    double getBoost() const { return m_boostInc; }
    /** @returns the current stored boost duration **/
    Time getBoostCharge() const { return m_boostCharge; }
    /** Adds boost charge but won't exceed maximum **/
    void addBoostCharge(Time charge);
    /** @returns the maximum stored boost duration **/
    Time getBoostMax() const;
    /** @returns the main color. **/
    sf::Color getPrimaryColor() const { return m_primaryColor; }
    /** @returns the secondary color used for effects. **/
    sf::Color getSecondaryColor() const { return m_secondaryColor; }
    /** Sets both the main and secondary color. **/
    void setColors(sf::Color c1, sf::Color c2) {
        m_primaryColor = c1;
        m_secondaryColor = c2;
    }
    /** Consumes food that will add both growth and some boost charge **/
    void eat(const Food& food);
    /** Changes state to Dying and will eventually become dead **/
    void die(const Time& elapsedTime);
    /** Drawable override. Draws all parts of the viper to the target **/
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    /** @returns The track the viper follows. **/
    const Track& getTrack() const { return m_track; }
    /** @returns The first point on the track the Viper is following. **/
    const TrackPoint* getHead() const { return m_headPoint; }
    /** @returns Normal (spatial) length of the Viper. **/
    double getLength() const;
    /** forawrds info from ViperCfg **/
    double getNominalWidth() const;
    /** Initiliazes the position and direction of the Viper given the specified
     * length. **/
    void setup(const Vec2& from, double angle, double numberOfBodySegments);
    /** Sets the speed. **/
    void setSpeed(double s) { m_speed = s; }
    /** @returns current speed. **/
    double getSpeed() const { return m_speed; }
    /** Sets the speed. **/
    void setNominalSpeed(double s) { m_nominalSpeed = s; }
    /** @returns current speed. **/
    double getnominalSpeed() const { return m_nominalSpeed; }
    /** Sets how fast the angle chanes as well as the boost factor **/
    void steer(double angularSpeed, double boost) {
        m_angularSpeed = angularSpeed;
        m_boostInc = boost;
    }
    /** @returns The temporal length of the Viper.
     * It is the time it takes for the tail to reach the current position of the
     * head. **/
    Time getTemporalLength() const { return m_temporalLength; }
    /** @returns the minimum turning radius **/
    double getMaxAngularSpeed() const;
    /** Updates state of the Viper. Should normally be called by the onNotify
     * member function. **/
    void update(Time elapsedTime);
    Vec2 getVelocity() const { return Vec2(m_speed, 0).rotate(m_angle); }

    /** Methods overriden from ColliderSegmented **/
    size_t getSegmentCount() const override;
    size_t getSegmentPointCount(size_t) const override { return 4; }
    Vec2 getSegmentGlobalPoint(size_t i, size_t j) const override;
    bool isSegmentActive(size_t i) const override;

  private:
    struct ViperConfiguration;
    static ViperConfiguration viperCfg;

    enum class ViperPart { Head, Body, Tail };
    TrackPoint* createNextHeadTrackPoint(Time elapsedTime);
    void cleanUpTrailingTrackPoints();
    void clearDinnerTimes();
    void grow(const Time& elapsedTime);

    void updateVertices();
    void updateVertices(ViperPart, Time timeFront, const Time& temporalLength);
    sf::Color calcVertexColor(Time time);

    void updateMotion(const Time& elapsedTime);
    void updateSpeed(const Time& elapsedTime);
    void updateAngle(const Time& elapsedTime);
    void updateBoostCharge(Time charge);

    double m_angle;         // degrees, clockwise since y-axis is downwards
    double m_angularSpeed;  // degrees/s
    double m_nominalSpeed;  // px/s
    double m_speed;         // px/s
    double m_targetSpeed;   // px/s
    double m_boostInc;      // Boost speed = (1 + m_boost) * nominal speed
    Time m_boostCharge;     // fraction [0., 1.]
    Time m_boostRechargeCooldown;  // Countdown from viperBoostChargeCooldown
    Time m_temporalLength;         // s
    Time m_growth;                 // s
    TrackPoint* m_headPoint;
    Track m_track;
    sf::Color m_primaryColor;
    sf::Color m_secondaryColor;
    std::map<Time, Time> m_dinnerTimes;

    std::vector<sf::Vertex> m_verticesHead;
    std::vector<sf::Vertex> m_verticesBody;
    std::vector<sf::Vertex> m_verticesTail;
};

}  // namespace VVipers
#endif  // VVIPERS_SCENES_GAMEELEMENTS_VIPER_HPP
