#ifndef VVIPER_VIPERPHYSICS_HPP
#define VVIPER_VIPERPHYSICS_HPP

#include <vector>
#include <SFML/System/Time.hpp>
#include <vvipers/Track.hpp>

namespace VVipers {

class ViperPhysics {
  public:
    ViperPhysics();
    float angle() const { return m_angle; }
    const Track& getTrack() const { return m_track; }
    void growth(const sf::Time& g);
    const TrackPoint* head() const { return m_head; }
    float length() const;
    void setAngle(float a) { m_angle = mod180Deg(a); }
    void setup(const Vec2f& from, float angle, const sf::Time& length);
    float speed() const { return m_speed; }
    sf::Time temporalLength() const { return m_temporalLength; }
    void update(const sf::Time& elapsedTime);

  private:
    TrackPoint* createNextHeadTrackPoint(sf::Time elapsedTime);
    void cleanUpTrailingTrackPoints();
    void grow(const sf::Time& elapsedTime);
    static const float s_nominalSpeed;  // px/s
    float m_angle;  // degrees, clockwise since y-axis is downwards
    float m_speed;  // px/s
    float m_acc;    // px/s²
    sf::Time m_temporalLength;
    sf::Time m_growth;
    TrackPoint* m_head;
    Track m_track;
};

}  // namespace VVipers
#endif