#ifndef VVIPERS_VECTORMATH_HPP
#define VVIPERS_VECTORMATH_HPP

#include <cmath>
#include <iostream>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <vvipers/VVMath.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace VVipers {

class Vec2 : public sf::Vector2<double> {
  public:
    Vec2(){};
    Vec2(double x, double y) : sf::Vector2<double>(x, y){};
    template <typename U>
    Vec2(const sf::Vector2<U>& v) : sf::Vector2<double>(v) {}
    double abs() const;
    double dot(const Vec2& v) const;
    Vec2& normalize(double norm = 1);
    Vec2 perpVec() const;
    double projectionScalar( Vec2 v ) const;
    Vec2 projectionVector( Vec2 v ) const;
    operator sf::Vector2f() { return sf::Vector2f(x, y); }
};

inline Vec2 operator+(const Vec2& left, const Vec2& right) {
    return Vec2(left.x + right.x, left.y + right.y);
}

inline Vec2 operator-(const Vec2& left, const Vec2& right) {
    return Vec2(left.x - right.x, left.y - right.y);
}

inline Vec2 operator*(const Vec2& left, const Vec2& right) {
    return Vec2(left.x * right.x, left.y * right.y);
}

template <typename T>
inline Vec2 operator*(const Vec2& left, T right) {
    return Vec2(left.x * right, left.y * right);
}

template <typename T>
inline Vec2 operator*(T left, const Vec2& right) {
    return Vec2(left * right.x, left * right.y);
}

inline Vec2 operator/(const Vec2& left, const Vec2& right) {
    return Vec2(left.x / right.x, left.y / right.y);
}

template <typename T>
inline Vec2 operator/(const Vec2& left, T right) {
    return Vec2(left.x / right, left.y / right);
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
    return os << "(x: " << v.x << ", y: " << v.y << ")";
}

inline double abs(const Vec2& v) { return v.abs(); }

inline double distance(const Vec2& a, const Vec2& b) { return abs(b - a); }

sf::Rect<double> rectangularBounds( const std::vector<Vec2>& coords );

}  // namespace VVipers

#endif