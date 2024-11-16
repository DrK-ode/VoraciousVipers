#ifndef VVIPERS_UTILITIES_VECTORMATH_HPP
#define VVIPERS_UTILITIES_VECTORMATH_HPP

#include <cmath>
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <vvipers/Utilities/VVMath.hpp>
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
    Vec2 normalized(double norm = 1) const;
    /** Rotates vector 90 degrees counter clockwise (clockwise on screen) **/
    Vec2 perpendicular() const;
    /** Rotates vector counter clockwise (clockwise on screen) **/
    Vec2& rotate(double rads);
    double projectionScalar( Vec2 v ) const;
    Vec2 projectionVector( Vec2 v ) const;
    double squared() const {return x*x+y*y;}
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

}  // namespace VVipers

#endif // VVIPERS_UTILITIES_VECTORMATH_HPP