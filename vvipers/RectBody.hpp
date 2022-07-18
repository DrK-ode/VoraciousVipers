#ifndef VVIPERS_RECTBODY_HPP
#define VVIPERS_RECTBODY_HPP

#include <vvipers/CollisionBody.hpp>
#include <vvipers/Vec2.hpp>
#include <string>

namespace VVipers{

class RectBody : public CollisionBody {
  public:
    RectBody(Vec2 topLeft, Vec2 size);
    void setColor(sf::Color color);
    void setColor(sf::Color c1, sf::Color c2, sf::Color c3, sf::Color c4);
    void setTextureCoords(Vec2 topLeft, Vec2 size);
    void setBodyPart(const std::string& label = "", bool active = false);
    private:
    using CollisionBody::clear;
    using CollisionBody::resize;
    using CollisionBody::setVertex;
    using CollisionBody::appendVertex;
    using CollisionBody::assignBodyParts;
};

}

#endif // VVIPERS_RECTBODY_HPP
