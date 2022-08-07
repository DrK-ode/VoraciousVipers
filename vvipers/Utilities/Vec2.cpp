#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

double Vec2::abs() const {
    return std::sqrt(this->x * this->x + this->y * this->y);
}

double Vec2::dot(const Vec2& v) const { return this->x * v.x + this->y * v.y; }

Vec2 Vec2::normalized(double norm) const { return *this * norm / this->abs(); }

Vec2 Vec2::perpVec() const { return Vec2(-this->y, this->x); }

Vec2& Vec2::rotate(double degree) {
    double rads = degToRad(degree);
    double s = std::sin(rads);
    double c = std::cos(rads);
    *this = Vec2(c * x - s * y, s * x + c * y);
    return *this;
}

double Vec2::projectionScalar(Vec2 v) const {
    double vAbs = v.abs();
    if (vAbs < 1e-9)
        return 0.;
    return this->dot(v) / vAbs;
}

Vec2 Vec2::projectionVector(Vec2 v) const {
    double v2 = v.dot(v);
    if (v2 == 0.)
        return v;
    return v * this->dot(v) / v2;
}

}  // namespace VVipers