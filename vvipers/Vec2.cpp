#include <vvipers/Vec2.hpp>

namespace VVipers {

double Vec2::abs() const {
    return std::sqrt(this->x * this->x + this->y * this->y);
}

double Vec2::dot(const Vec2& v) const { return this->x * v.x + this->y * v.y; }

Vec2& Vec2::normalize(double norm) {
    norm /= this->abs();
    this->x *= norm;
    this->y *= norm;
    return (*this);
}

Vec2 Vec2::perpVec() const { return Vec2(-this->y, this->x); }

}  // namespace VVipers