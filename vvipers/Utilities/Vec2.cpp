#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

double Vec2::abs() const {
    return std::sqrt(this->x * this->x + this->y * this->y);
}

double Vec2::dot(const Vec2& v) const { return this->x * v.x + this->y * v.y; }

Vec2 Vec2::normalized(double norm) const {
    return *this * norm / this->abs();
}

Vec2 Vec2::perpVec() const { return Vec2(-this->y, this->x); }

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

sf::Rect<double> rectangularBounds( const std::vector<Vec2>& coords ) {
    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();

    for (auto point : coords ) {
        xmin = std::min(xmin, point.x);
        xmax = std::max(xmax, point.x);
        ymin = std::min(ymin, point.y);
        ymax = std::max(ymax, point.y);
    }
    return sf::Rect<double>(xmin, ymin, xmax - xmin, ymax - ymin);
}

}  // namespace VVipers