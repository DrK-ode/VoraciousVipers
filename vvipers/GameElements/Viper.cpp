#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>
#include <memory>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/Utilities/VVColor.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>
#include <vvipers/config.hpp>

#include "vvipers/Collisions/CollidingSegment.hpp"
#include "vvipers/GameElements/Track.hpp"

namespace VVipers {

using namespace std::chrono_literals;

/** Nested helper class. Viper class holds one static instance. **/
class Viper::ViperConfiguration {
    friend class Viper;

  private:
    void initialize(const OptionsProvider& options,
                    const TextureProvider& textures) {
        nominal_speed = options.getOptionDouble("Viper/nominalSpeed");
        nominal_segment_width =
            options.getOptionDouble("Viper/nominalSegmentWidth");  // px
        boost_max_charge = timeFromSeconds(
            options.getOptionDouble("Viper/boostMaxCharge"));  // s
        boost_recharge_rate =
            options.getOptionDouble("Viper/boostRechargeRate");  // s per s
        boost_recharge_cooldown = timeFromSeconds(options.getOptionDouble(
            "Viper/boostRechargeCooldown"));  // Countdown start

        head_nominal_length = options.getOptionDouble(
            "ViperModel/ViperHead/nominalLength");  // px
        head_duration =
            timeFromSeconds(head_nominal_length / nominal_speed);  // s
        head_nodes =
            options.getOption2DVectorArray("ViperModel/ViperHead/nodes");

        body_nominal_length = options.getOptionDouble(
            "ViperModel/ViperBody/nominalLength");  // px
        body_duration =
            timeFromSeconds(body_nominal_length / nominal_speed);  // s
        body_nodes =
            options.getOption2DVectorArray("ViperModel/ViperBody/nodes");

        tail_nominal_length = options.getOptionDouble(
            "ViperModel/ViperTail/nominalLength");  // px
        tail_duration =
            timeFromSeconds(tail_nominal_length / nominal_speed);  // s
        tail_nodes =
            options.getOption2DVectorArray("ViperModel/ViperTail/nodes");

        head_texture = textures.getTexture("ViperHead");
        body_texture = textures.getTexture("ViperBody");
        tail_texture = textures.getTexture("ViperTail");

        // Find for which index the head is the widest
        last_active_index = 0;
        double wMax = 0;
        for (size_t i = 0; i < head_nodes.size(); ++i) {
            if (std::abs(head_nodes[i].x) >= wMax) {
                last_active_index = i;
                wMax = std::abs(head_nodes[i].x);
            }
        }

        initialized = true;
    }

  public:
    bool initialized = false;
    double nominal_speed;          // px/s
    double nominal_segment_width;  // px
    Time boost_max_charge;         // s
    double boost_recharge_rate;    // s per s
    Time boost_recharge_cooldown;  // Countdown start

    size_t last_active_index;

    double head_nominal_length;  // px
    Time head_duration;          // s
    std::vector<Vec2> head_nodes;
    const sf::Texture* head_texture;

    double body_nominal_length;  // px
    Time body_duration;          // s
    std::vector<Vec2> body_nodes;
    const sf::Texture* body_texture;

    double tail_nominal_length;  // px
    Time tail_duration;          // s
    std::vector<Vec2> tail_nodes;
    const sf::Texture* tail_texture;
};

Viper::ViperConfiguration Viper::_viperCfg;

Viper::Viper(const OptionsProvider& options, const TextureProvider& textures)
    : _boost_increase(0.), _boost_recharge_cooldown(0.), _growth(0.) {
    if (!_viperCfg.initialized)
        _viperCfg.initialize(options, textures);
    _boost_charge = _viperCfg.boost_max_charge;

    _nominalSpeed = _viperCfg.nominal_speed;
    _speed = _nominalSpeed;
}

std::vector<std::shared_ptr<const CollidingSegment>> Viper::colliding_segments()
    const {
    std::vector<std::shared_ptr<const CollidingSegment>> vec;
    std::vector<Vec2> corners;
    // Vertex order is like a triangle strip while we need the CollidingPolygon
    // to get its corners in clockwise order. So we first extract the even index
    // points, and then the odd.
    for (uint parity = 1; parity >= 0; --parity) {
        for (size_t vertex = 0; vertex < _vertices_head.size(); vertex += 2) {
            corners.emplace_back(_vertices_head[vertex + parity].position);
        }
    }
    // Head always get ID 0
    vec.emplace_back(new CollidingPolygon(corners, this, 0));

    size_t n_nodes_per_segment = _viperCfg.body_nodes.size();
    size_t n_body_segments =
        (_vertices_body.size() - 2) / (n_nodes_per_segment - 2);
    for (size_t i = 0; i < n_body_segments; ++i) {
        corners.clear();
        for (uint parity = 1; parity >= 0; --parity) {
            size_t start_index = i*n_nodes_per_segment - 2;
            for (size_t vertex = 0; vertex < n_nodes_per_segment; vertex += 2) {
                corners.emplace_back(_vertices_head[start_index + vertex + parity].position);
            }
        }
        // Body segments get indices starting from 1
        vec.emplace_back(new CollidingPolygon(corners, this, 1 + i));
    }

    for (uint parity = 1; parity >= 0; --parity) {
        for (size_t vertex = 0; vertex < _vertices_tail.size(); vertex += 2) {
            corners.emplace_back(_vertices_tail[vertex + parity].position);
        }
    }
    // Tail gets last index
    vec.emplace_back(new CollidingPolygon(corners, this, vec.size()));

    return vec;
}

void Viper::eat(const Food& food) {
    add_growth(_viperCfg.body_duration * (food.getRadius() * food.getRadius()) /
                   (Food::nominalFoodRadius * Food::nominalFoodRadius),
               _track->head_time(), food.color());
    add_boost_charge(0.5s);
}

void Viper::die(const Time& elapsedTime) {
    _temporal_length -= 4 * elapsedTime;
    if (_temporal_length <= timeFromSeconds(0))
        state(Dead);
}

void Viper::add_growth(Time howMuch, Time when, sf::Color color) {
    _dinner_times[when] = {howMuch, color};
}

double Viper::length() const {
    return _track->length(_track->head_time(),
                          _track->head_time() - _temporal_length);
}

double Viper::nominal_width() const { return _viperCfg.nominal_segment_width; }

void Viper::setup(const Vec2& tailPosition, double angle,
                  double numberOfBodySegments) {
    _angle = angle;
    _temporal_length = _viperCfg.head_duration + _viperCfg.tail_duration;
    _growth = numberOfBodySegments * _viperCfg.body_duration;

    Vec2 direction = Vec2(1, 0).rotate_deg(angle);
    double length = timeAsSeconds(_temporal_length) * speed();
    auto viperVector = length * direction;
    _track = std::unique_ptr<TemporalTrack>(
        new TemporalTrack(tailPosition + viperVector, _temporal_length,
                          tailPosition, timeFromSeconds(0)));
}

void Viper::create_next_head_temporal_track_point(Time elapsedTime) {
    _track->create_front(Vec2(_speed, 0).rotate_deg(_angle), elapsedTime);
}

void Viper::clean_up_trailing_temporal_track_points() {
    // Removes any point(s) the whole Viper has passed through
    _track->remove_trailing(_track->head_time() - _temporal_length);
}

void Viper::clean_up_dinner_times() {
    // Slightly wasteful but there will never be more than a handful dinner
    // times stored at the same time.
    for (auto& dinnerTime : _dinner_times)
        if (dinnerTime.first < _track->tail_time()) {
            _growth += dinnerTime.second.amount;
            dinnerTime.second.amount = timeFromSeconds(0);
        }
    // This loop can only remove a maximum of one time per update. But that's
    // fine
    for (auto& dinnerTime : _dinner_times)
        // Times 10 give us some margin but it would be nicer to specify
        // exactly...
        if (dinnerTime.first + 10 * _viperCfg.body_duration <
            _track->tail_time()) {
            _dinner_times.erase(dinnerTime.first);
            break;
        }
}

Time Viper::boost_max() const { return _viperCfg.boost_max_charge; }

void Viper::grow(const Time& elapsedTime) {
    // Limit the growth to how much time that has passed
    Time actualGrowth = std::min(_growth, elapsedTime);
    _temporal_length += actualGrowth;
    _growth -= actualGrowth;
}

// This allows the viper narrower turns when not boosting
double Viper::max_angular_speed() const {
    return _nominalSpeed / _viperCfg.nominal_segment_width;
}

void Viper::update(Time elapsedTime) {
    if (state() == Dead) {
        DestroyEvent event(this);
        notify(&event);
        return;
    }
    if (state() == Dying)
        die(elapsedTime);  // Allow the viper to die for a while
    else {
        create_next_head_temporal_track_point(elapsedTime);
        grow(elapsedTime);
    }
    update_motion(elapsedTime);
    update_vertices();
    clean_up_trailing_temporal_track_points();
    clean_up_dinner_times();
}

void Viper::add_boost_charge(Time charge) {
    auto oldCharge = _boost_charge;
    _boost_charge += charge;
    _boost_charge = std::max(_boost_charge, timeFromSeconds(0));
    _boost_charge = std::min(_boost_charge, boost_max());

    if (_boost_charge != oldCharge) {
        ObjectModifiedEvent event(this);
        notify(&event);
    }
}

void Viper::update_boost_charge(Time elapsedTime) {
    if (_boost_recharge_cooldown == timeFromSeconds(0)) {
        add_boost_charge(elapsedTime * _viperCfg.boost_recharge_rate);
    } else if (_boost_increase > 0)
        add_boost_charge(-elapsedTime);
}

void Viper::update_speed(const Time& elapsedTime) {
    double acceleration = 0;
    double targetSpeed = _viperCfg.nominal_speed;
    if (_boost_increase > 0) {
        targetSpeed *= (1 + _boost_increase);
        _boost_recharge_cooldown = _viperCfg.boost_recharge_cooldown;
    } else {
        _boost_recharge_cooldown -=
            std::min(_boost_recharge_cooldown, elapsedTime);
    }
    update_boost_charge(elapsedTime);
    if (_speed < targetSpeed) {
        // 0.5s to increase speed by nominal speed but cap at targetSpeed
        acceleration =
            std::min(2 * _viperCfg.nominal_speed,
                     (targetSpeed - _speed) / timeAsSeconds(elapsedTime));
    } else if (_speed > targetSpeed) {
        acceleration =
            std::max(-_viperCfg.nominal_speed,
                     (targetSpeed - _speed) / timeAsSeconds(elapsedTime));
    }
    _speed += acceleration * timeAsSeconds(elapsedTime);
}

void Viper::update_angle(const Time& elapsedTime) {
    _angle += _angularSpeed * timeAsSeconds(elapsedTime);
}

void Viper::update_motion(const Time& elapsedTime) {
    update_speed(elapsedTime);
    update_angle(elapsedTime);
}

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = _viperCfg.head_texture;
    target.draw(&_vertices_head[0], _vertices_head.size(),
                sf::PrimitiveType::TriangleStrip, states);
    states.texture = _viperCfg.body_texture;
    target.draw(&_vertices_body[0], _vertices_body.size(),
                sf::PrimitiveType::TriangleStrip, states);
    states.texture = _viperCfg.tail_texture;
    target.draw(&_vertices_tail[0], _vertices_tail.size(),
                sf::PrimitiveType::TriangleStrip, states);
}

void Viper::update_vertices() {
    Time headDuration = std::min(_temporal_length, _viperCfg.head_duration);
    if (_temporal_length < _viperCfg.head_duration + _viperCfg.tail_duration) {
        headDuration = _temporal_length *
                       (_viperCfg.head_duration /
                        (_viperCfg.head_duration + _viperCfg.tail_duration));
    }
    Time tailDuration =
        std::min(_temporal_length - headDuration, _viperCfg.tail_duration);
    Time bodyDuration = _temporal_length - headDuration - tailDuration;

    Time headFront = _track->head_time();
    Time bodyFront = headFront - headDuration;
    Time tailFront = bodyFront - bodyDuration;

    update_vertices_for_body_part(ViperPart::Head, headFront, headDuration);
    update_vertices_for_body_part(ViperPart::Body, bodyFront, bodyDuration);
    update_vertices_for_body_part(ViperPart::Tail, tailFront, tailDuration);
}

// I hate this code. It needs to be divided into smaller manageble pieces.
void Viper::update_vertices_for_body_part(ViperPart part, Time timeFront,
                                          const Time& temporalLength) {
    /* This code does the following in order to calculate the vertices for one
     * parts of the viper. It does mainly the same thing but not exactly
     * depending on which kind of parts it is. Since there might be several body
     * segments the exceptions are mostly here.
     * 1) Checks what kind of snake
     *    segment we are working with and sets a few variables based on this.
     * 2) Calculates the position and texture coordinates for all segments (only
     *    one for head and tail).
     *       2a) If making several segments the two first vertices are skipped
     *           over for all segments except the first one.
     *      2b) Any left over temporal length is put in a slightly smaller body
     *          segment in the end.
     * 3) Assigns bodyparts to be used by the collision system. Only the first
     *    parts of the head are "active".
     */
    if (temporalLength <= timeFromSeconds(0))
        return;

    // True width is calculated later and proportional to dL/dt
    double nominalWidth = _viperCfg.nominal_segment_width;
    // Determining model, texture and lengths depending on the type of segments
    const std::vector<Vec2>* sketch;
    std::vector<sf::Vertex>* vertices;
    Vec2 textureSize;
    int numberOfSegments = 1;
    Time segmentLength = temporalLength;
    switch (part) {
        case ViperPart::Head: {
            sketch = &_viperCfg.head_nodes;
            vertices = &_vertices_head;
            textureSize = _viperCfg.head_texture->getSize();
            break;
        }
        case ViperPart::Body: {
            sketch = &_viperCfg.body_nodes;
            vertices = &_vertices_body;
            textureSize = _viperCfg.body_texture->getSize();
            const Time nominalSegmentDuration = _viperCfg.body_duration;
            segmentLength = std::min(temporalLength, nominalSegmentDuration);
            // The growing segment starts from 0 and has a fraction of the
            // nominal segment length
            numberOfSegments = temporalLength / nominalSegmentDuration + 1;
            break;
        }
        case ViperPart::Tail: {
            sketch = &_viperCfg.tail_nodes;
            vertices = &_vertices_tail;
            textureSize = _viperCfg.tail_texture->getSize();
            break;
        }
    }
    vertices->clear();

    // Calc position and texture coordinates
    for (int iSeg = 0; iSeg < numberOfSegments; ++iSeg) {
        Time actualLength = segmentLength;
        // If it is the last segment: use up whatever is left instead
        if (iSeg == numberOfSegments - 1)
            actualLength =
                temporalLength - segmentLength * (numberOfSegments - 1);
        // Start on 0 if it is the first segment, otherwise on 2
        for (size_t iNode = (iSeg == 0) ? 0 : 2; iNode < sketch->size();
             iNode += 2) {
            // How far we have come so far.
            Time time = timeFront - sketch->at(iNode).y * actualLength;
            // This is the position at the snake axis
            Vec2 mid = _track->position(time);
            // This vector reaches out the the side of the snake
            Vec2 velocity = _track->velocity(time);
            double speed = velocity.abs();
            Vec2 perp = velocity.perpendicular() *
                        (_viperCfg.nominal_speed / (speed * speed) *
                         nominalWidth * sketch->at(iNode).x);
            // Use the whole width of the texture and repeat the texture if
            // there are several segments
            Vec2 textCoords1 =
                (Vec2(0.5f, iSeg) + sketch->at(iNode)) * textureSize;
            Vec2 textCoords2 =
                (Vec2(0.5f, iSeg) + sketch->at(iNode + 1)) * textureSize;
            // All vertices have the same color atm
            sf::Color color = calculate_vertex_color(time);
            vertices->push_back({mid + perp, color, textCoords1});
            vertices->push_back({mid - perp, color, textCoords2});
        }
        timeFront -= actualLength;  // Next segment will start here
    }
}

sf::Color Viper::calculate_vertex_color(Time time) {
    // TODO: Loop through DinnerTimes instead of calling for every vertex.

    auto headSide = _dinner_times.lower_bound(time);  // Closer to head
    auto tailSide = headSide != _dinner_times.cbegin() ? std::prev(headSide)
                                                       : _dinner_times.cend();

    double sFactor1 = 0;
    sf::Color color1 = sf::Color::Transparent;
    if (tailSide != _dinner_times.end()) {
        Time half_length = tailSide->second.amount;
        Time mid_point = tailSide->first + 0.5 * half_length;
        Time distance = std::chrono::abs(time - mid_point);
        if (distance < 0.5 * half_length) {
            color1 = tailSide->second.color;
            sFactor1 = 1. - distance / half_length;
        }
    }
    double sFactor2 = 0;
    sf::Color color2 = sf::Color::Transparent;
    if (headSide != _dinner_times.end()) {
        Time half_length = headSide->second.amount;
        Time mid_point = headSide->first + 0.5 * half_length;
        Time distance = std::chrono::abs(time - mid_point);
        if (distance < 0.5 * half_length) {
            color1 = headSide->second.color;
            sFactor1 = 1. - distance / half_length;
        }
    }
    double sumFactor = sFactor1 + sFactor2;
    if (sumFactor > 1) {
        sFactor1 = sFactor1 / sumFactor;
        sFactor2 = sFactor2 / sumFactor;
    }
    // Blends the main color with the blend of the two food colors (normally
    // only one will be present)
    return blendColors(_primaryColor, 1. - sumFactor,
                       blendColors(color1, sFactor1, color2, sFactor2),
                       sumFactor);
}

}  // namespace VVipers