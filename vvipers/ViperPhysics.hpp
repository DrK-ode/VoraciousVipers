#ifndef VVIPER_VIPERPHYSICS_HPP
#define VVIPER_VIPERPHYSICS_HPP

#include <vector>
#include <vvipers/Collidable.hpp>
#include <vvipers/Time.hpp>
#include <vvipers/Track.hpp>

namespace VVipers {
/**
 * A physical model of a Viper (position, speed etc.).
 * Handles all aspects of the Viper that is not related to the graphical
 * representation.
 */
class ViperPhysics : public Collidable {
  public:
    ViperPhysics();
    /** @return current direction of the head. **/
    double angle() const { return m_angle; }
    /** @return The track the viper follows. **/
    const Track& getTrack() const { return m_track; }
    /** Adds time the Viper should spend growing. **/
    void growth(const Time& g);
    /** @return The first (front) point on the track the Viper is following. **/
    const TrackPoint* head() const { return m_head; }
    /** @return Normal (spatial) length of the Viper. **/
    double length() const;
    /** Sets the direction of the head. **/
    void setAngle(double a) { m_angle = mod180Deg(a); }
    /** Initiliazes the position and direction of the Viper given the specified
     * length. **/
    void setup(const Vec2& from, double angle, const Time& length);
    /** @return current speed. **/
    double speed() const { return m_speed; }
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

    void updateNodes();
    void updateNodes(const Time& timeFront, const Time& temporalLength,
                     const std::vector<Vec2>& relativePosistion,
                     std::vector<Vec2>& nodeVector);

    static const double s_nominalSpeed;  // px/s
    double m_angle;  // degrees, clockwise since y-axis is downwards
    double m_speed;  // px/s
    double m_acc;    // px/s²
    Time m_temporalLength;
    Time m_growth;
    TrackPoint* m_head;
    Track m_track;
    std::vector<Vec2> m_headNodes;
    std::vector<Vec2> m_bodyNodes;
    std::vector<Vec2> m_tailNodes;
    std::vector<Vec2> m_headEdges;
    std::vector<Vec2> m_bodyEdges;
    std::vector<Vec2> m_tailEdges;
    std::vector<Vec2> m_headNormals;
    std::vector<Vec2> m_bodyNormals;
    std::vector<Vec2> m_tailNormals;
};

}  // namespace VVipers
#endif