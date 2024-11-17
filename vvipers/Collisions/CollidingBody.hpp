#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <vvipers/Utilities/Vec2.hpp>

#include "vvipers/Utilities/Shape.hpp"

namespace VVipers {

class CollidingBody {
  public:
    CollidingBody(const std::string& str) : _name(str) {}
    virtual ~CollidingBody() {}
    virtual std::shared_ptr<const Shape> segment_shape(size_t index) const = 0;
    std::string name() const { return _name; }
    virtual size_t number_of_segments() const = 0;
    void set_name(const std::string& str) { _name = str; }
    bool operator==(const CollidingBody& other) const { return this == &other; }

  private:
    std::string _name;
};

}  // Namespace VVipers
